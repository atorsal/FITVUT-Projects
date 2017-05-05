#include <stdio.h>
#include<unistd.h>
#include<thread>
#include<queue>
#include<mutex>
#include<vector>
#include <iostream>
#include<string.h>
#include<regex>




std::vector<std::mutex *> zamky_in, zamky_out; /* pole zamku promenne velikosti */

int line_score;
std::mutex score_mutex;
int res;

char *line;

char *read_line(int *res) {
	std::string line;
	char *str;
	if (std::getline(std::cin, line)) {
		str=(char *) malloc(sizeof(char)*(line.length()+1));
		strcpy(str,line.c_str());
		*res=1;
		return str;
	} else {
		*res=0;
		return NULL;
	}

}


void f(char *re, int re_score, std::mutex *in, std::mutex *out) {
	std::regex rex(re);
	(*in).lock();
	while(res) {
		if(std::regex_match(line,rex)) {
			score_mutex.lock();
			line_score=line_score+re_score;
			score_mutex.unlock();
		}
		(*out).unlock();
		(*in).lock();
	}
}

int main(int argc, char* argv[]) {
	if ((argc<4)||((argc%2)==1)) {
		printf("USAGE: pgrep MIN_SCORE RE1 SC1 [ RE2 SC2 ] [ RE3 SC3 ] ...\n");
		return 1;
	}
	int num=(argc-2)/2;
	std::vector <std::thread *> threads; /* pole threadu promenne velikosti */

	int fin_score=atoi(argv[1]);

	/* vytvorime zamky */
	zamky_in.resize(num); 
	zamky_out.resize(num); 
	for(int i=0;i<num;i++){	
		std::mutex *new_zamek = new std::mutex();
		zamky_in[i]=new_zamek;
		(*(zamky_in[i])).lock();
		std::mutex *new_zamek2 = new std::mutex();
		zamky_out[i]=new_zamek2;
		(*(zamky_out[i])).lock();
	}

	/* vytvorime thready */
	threads.resize(num); /* nastavime si velikost pole threads */
	for(int i=0;i<num;i++){	
		std::thread *new_thread = new std::thread (f,argv[2+(i*2)],atoi(argv[3+(i*2)]),zamky_in[i],zamky_out[i]);
		threads[i]=new_thread;
	}
	/* ---------------------- */
	/* nacteme a vypiseme radky */
	line=read_line(&res);
	while (res) {
		line_score=0;
		for(int i=0;i<num;i++) (*(zamky_in[i])).unlock();

		/* waiting for results */
		for(int i=0;i<num;i++) (*(zamky_out[i])).lock();
		if (line_score >= fin_score) 
			printf("%s\n",line);
		
		free(line); /* uvolnim pamet */
		line=read_line(&res);
	}
	/* ---------------------- */
	for(int i=0;i<num;i++) (*(zamky_in[i])).unlock();


	/* provedeme join a uvolnime pamet threads */
	for(int i=0;i<num;i++){
		(*(threads[i])).join();
		delete threads[i];
	}
	/* uvolnime pamet zamku */
	for(int i=0;i<num;i++){
		delete zamky_in[i];
		delete zamky_out[i];
	}

	
}
