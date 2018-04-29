#include "types.h"
#include "stat.h"
#include "user.h"
//#include <string.h>
#include "getopt.c"
//#include <unistd.h>

char buf[512];

/*which counts to print*/
short int print_lines, print_words, print_chars, print_defualt, print_max_line_length;
int total_lines, total_words, total_chars, global_max_line_length=0;

void printcounts(int lines, int words, int chars, int max_line_length, char *name) {
	if (print_lines) {
		printf(1, "%d ", lines);
	}
	if (print_words) {
		printf(1, "%d ", words);
	}
	if (print_chars) {
		printf(1, "%d ", chars);
	}
	if (print_max_line_length) {
		printf(1, "%d ", max_line_length);
	}
	if (print_defualt) {
		printf(1, "%d %d %d ", lines, words, chars, name);
	}
	printf(1,"%s\n", name);
}


void display_usage() {
	printf(1,"Documentation\n");
	/*printf(1,"wc - print newline, word and byte counts for each file \n
		USAGE \n:
		wc [OPTION]... [FILE]...
		DESCRIPTION \n:
		Print newline, word and byte counts for each FILE,
		and a total line if more than one FILES is specified.
		A word is a non-zero-length  sequence of characters delimited by white space.
		");
		*/
}


void
wc(int fd, char *name)
{
  int i, n;
  int l, w, c, inword, line_beg, max_line_length=0;

  l = w = c = 0;
  inword = 0;
  line_beg=0;
  while((n = read(fd, buf, sizeof(buf))) > 0){
	for(i=0; i<n; i++){
	  c++;
	  if(buf[i] == '\n') {
		if (c-line_beg-1>max_line_length) {
			max_line_length=c-line_beg-1;
			line_beg=c;
		}
		l++;
	   }
	  if(strchr(" \r\t\n\v", buf[i]))
		inword = 0;
	  else if(!inword){
		w++;
		inword = 1;
	  }
	}
  }

  if(n < 0){
	printf(1, "wc: read error\n");
	exit();
  }

  printcounts(l,w,c,max_line_length,name);

  total_chars+=c;
  total_words+=w;
  total_lines+=l;
  if (global_max_line_length<max_line_length)
	global_max_line_length=max_line_length;
}


int
main(int argc, char *argv[])
{
  int fd, i;
  int optc;

  if(argc <= 1){
	wc(0, "");
	exit();
  }

  print_lines=print_words=print_chars=print_max_line_length=0;
  print_defualt=1;
  
  //i=1;
  while ((optc=getopt(argc, argv, "lwcL"))!=-1) {
  	if (optc==-2) {
  		continue;
  	}
	//printf(1, "optc:%c\n", optc);
	switch (optc) {
		case 'c':
			print_chars=1;
			print_defualt=0;
			break;
		case 'w':
			print_words=1;
			print_defualt=0;
			break;
		case 'l':
			//printf(1,"print_lines flag\n");
			print_lines=1;
			print_defualt=0;
			break;
		case 'L':
			print_max_line_length=1;
			print_defualt=0;
			break;
		default:
			display_usage();
		    exit();
	}
  }
 
  //i=no_option_fields+1;
  //printf(1, "Flags:%d %d %d %d %d %d\n", print_lines, print_words, print_chars, print_max_line_length, print_defualt, i);

  int tot_flag=0;
  for(i=1; i < argc; i++){
  	if (argv[i][0]!='-') {
		if((fd = open(argv[i], 0)) < 0){
	 	 	printf(1, "wc: cannot open %s\n", argv[i]);
	  		exit();
		}

		wc(fd, argv[i]);
		tot_flag++;
		close(fd);
  	}
  }
  if (tot_flag>1) {
	printcounts(total_lines, total_words, total_chars, global_max_line_length, "Total");
  }
  exit();
}
