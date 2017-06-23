/**
* 2. projekt - Operační systémy
* Autor: David Bolvansky (xbolva00)
* Roller Coaster
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

/** Semaphore IDs*/
#define SEMAPHORE_1 "/xbolva00_sem_1"
#define SEMAPHORE_2 "/xbolva00_sem_2"
#define SEMAPHORE_3 "/xbolva00_sem_3"
#define SEMAPHORE_4 "/xbolva00_sem_4"
#define SEMAPHORE_5 "/xbolva00_sem_5"
#define SEMAPHORE_6 "/xbolva00_sem_6"
#define SEMAPHORE_7 "/xbolva00_sem_7"

/** Output file name */
#define LOGFILE "proj2.out"

/** Error codes */
enum error_codes { OK, INPUT_ERROR, SYSTEM_CALL_ERROR };

/** Semaphores */
sem_t *mutex, *mutex2, *boardingQueue, *unboardingQueue, *allAboard,
    *allAshore, *writing;

/** Shared counters */
int *actions_counter = NULL, actions_counter_id = 0;
int *boarders_counter = NULL, boarders_counter_id = 0;
int *unboarders_counter = NULL, unboarders_counter_id = 0;

/** Output file */
FILE *output = NULL;

/** Structure which contains required data to work with */
typedef struct params {
	int P;			// počet procesů reprezentujících pasažéra; P > 0.
	int C;			// kapacita vozíku; C > 0, P > C, P musí být vždy násobkem C.
	int PT;			// maximální hodnota doby (v milisekundách), po které je generován nový proces pro pasažéra; PT >= 0 && PT < 5001.
	int RT;			// maximální hodnota doby (v milisekundách) průjezdu trati; RT >= 0 && RT < 5001.
	int error_code;		// chybovy kod
} Params;


/** Function prototypes */
int init_semaphores();
void free_semaphores();
int init_resources();
void free_resources();
void kill_self();
void free_all();
void kill_process(pid_t process_id);
Params process_args(char *argv[]);

/**
* @brief Process arguments from input
* @param argv Array of arguments
* @return structure with parsed values
*/
Params process_args(char *argv[])
{
	Params data;
	data.error_code = INPUT_ERROR;
	char *first_error = NULL;
	char *second_error = NULL;
	char *third_error = NULL;
	char *fourth_error = NULL;

	int P = 0, C = 0, PT = 0, RT = 0;
    /** Convert strings to numbers */
	P = strtol(argv[1], &first_error, 10);
	C = strtol(argv[2], &second_error, 10);
	PT = strtol(argv[3], &third_error, 10);
	RT = strtol(argv[4], &fourth_error, 10);

    /** If any error, exit program */
	if (*first_error || *second_error || *third_error) {
		return data;
	}

	if (P <= 0 || C <= 0 || P <= C || (P % C != 0)) {
		return data;

	}

	if (PT < 0 || PT >= 5001) {
		return data;

	}
    
    if (RT < 0 || RT >= 5001) {
		return data;

	}

    /** Store values to structure */
	data.P = P;
	data.C = C;
	data.PT = PT;
	data.RT = RT;
	data.error_code = OK;
	return data;

}

/**
* @brief Initialize semaphores
* @return if success - 0 or if system call error - 2
*/
int init_semaphores()
{

	int SEM_FLAGS = O_CREAT | O_EXCL;
	if ((mutex =
	     sem_open(SEMAPHORE_1, SEM_FLAGS, 0666, 1)) == SEM_FAILED) {
		return SYSTEM_CALL_ERROR;
	}

	if ((mutex2 =
	     sem_open(SEMAPHORE_2, SEM_FLAGS, 0666, 0)) == SEM_FAILED) {
		return SYSTEM_CALL_ERROR;
	}

	if ((boardingQueue =
	     sem_open(SEMAPHORE_3, SEM_FLAGS, 0666, 0)) == SEM_FAILED) {
		return SYSTEM_CALL_ERROR;
	}

	if ((unboardingQueue =
	     sem_open(SEMAPHORE_4, SEM_FLAGS, 0666, 0)) == SEM_FAILED) {
		return SYSTEM_CALL_ERROR;
	}

	if ((allAboard =
	     sem_open(SEMAPHORE_5, SEM_FLAGS, 0666, 0)) == SEM_FAILED) {
		return SYSTEM_CALL_ERROR;
	}

	if ((allAshore =
	     sem_open(SEMAPHORE_6, SEM_FLAGS, 0666, 0)) == SEM_FAILED) {
		return SYSTEM_CALL_ERROR;
	}

	if ((writing =
	     sem_open(SEMAPHORE_7, SEM_FLAGS, 0666, 1)) == SEM_FAILED) {
		return SYSTEM_CALL_ERROR;
	}

	return OK;
}

/**
* @brief Initialize resources in shared memory
* @return if success - 0, if system call error - 2
*/
int init_resources()
{
	if ((actions_counter_id =
	     shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1) {
		return SYSTEM_CALL_ERROR;
	}

	if ((actions_counter =
	     (int *) shmat(actions_counter_id, NULL, 0)) == NULL) {
		return SYSTEM_CALL_ERROR;
	}

	if ((boarders_counter_id =
	     shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1) {
		return SYSTEM_CALL_ERROR;
	}

	if ((boarders_counter =
	     (int *) shmat(boarders_counter_id, NULL, 0)) == NULL) {
		return SYSTEM_CALL_ERROR;
	}

	if ((unboarders_counter_id =
	     shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1) {
		return SYSTEM_CALL_ERROR;
	}

	if ((unboarders_counter =
	     (int *) shmat(unboarders_counter_id, NULL, 0)) == NULL) {
		return SYSTEM_CALL_ERROR;
	}
	return OK;
}

/**
* @brief Free all, ie. semaphores and resources in shared memory
*/
void free_all()
{
	free_semaphores();
	free_resources();
}

/**
* @brief Free semaphores
*/
void free_semaphores()
{
	/** Close semaphores */
	sem_close(mutex);
	sem_close(mutex2);
	sem_close(boardingQueue);
	sem_close(unboardingQueue);
	sem_close(allAboard);
	sem_close(allAshore);
	sem_close(writing);

	/** Unlink semaphores */
	sem_unlink(SEMAPHORE_1);
	sem_unlink(SEMAPHORE_2);
	sem_unlink(SEMAPHORE_3);
	sem_unlink(SEMAPHORE_4);
	sem_unlink(SEMAPHORE_5);
	sem_unlink(SEMAPHORE_6);
	sem_unlink(SEMAPHORE_7);
}

/**
* @brief Free resources in shared memory
*/
void free_resources()
{

    /** Close output file */
	if (output != NULL) {
		fclose(output);
		output = NULL;
	}

    /** Detach resources in shared memory */
	shmdt(actions_counter);
	shmdt(boarders_counter);
	shmdt(unboarders_counter);

    /** Remove resources in shared memory */
	shmctl(actions_counter_id, IPC_RMID, NULL);
	shmctl(boarders_counter_id, IPC_RMID, NULL);
	shmctl(unboarders_counter_id, IPC_RMID, NULL);
}


/**
* @brief Kill self (process 0)
*/
void kill_self()
{
	free_all();
	kill(0, SIGTERM);
	exit(SYSTEM_CALL_ERROR);
}

/**
* @brief Kill specific process
* @param process_id ID of process to be killed
*/
void kill_process(pid_t process_id)
{

	free_all();
	kill(process_id, SIGTERM);
	exit(SYSTEM_CALL_ERROR);
}

/**
* @brief Main program
* @param argc Number of arguments
* @param argv Array of arguments
* @return if success - 0, if input (arguments) error - 1, if system call error - 2
*/
int main(int argc, char *argv[])
{
	/** Make streams unbuffered */
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

    /** Catch signals and call our function */
	signal(SIGTERM, kill_self);
	signal(SIGINT, kill_self);

    /** Check if we have all needed arguments */
	if (argc != 5) {
		fprintf(stderr,
			"Invalid arguments. Run with: ./proj2 P C PT RT\n");
		return INPUT_ERROR;
	}

	Params params;
    /** Process arguments */
	params = process_args(argv);

    /** Exit if any error*/
	if (params.error_code != 0) {
		fprintf(stderr, "Wrong arguments.\n");
		return INPUT_ERROR;
	}

	if (init_semaphores() == SYSTEM_CALL_ERROR) {
		fprintf(stderr, "Semaphores inicialization failed.\n");
		free_all();
		return SYSTEM_CALL_ERROR;
	}

	if (init_resources() == SYSTEM_CALL_ERROR) {
		fprintf(stderr, "Resources inicialization failed.\n");
		free_all();
		return SYSTEM_CALL_ERROR;
	}

    /** Open output file in write mode */
	if ((output = fopen(LOGFILE, "w")) == NULL) {
		fprintf(stderr, "Failed to open file %s\n", LOGFILE);
		free_all();
		return INPUT_ERROR;
	}

	/** Make output file unbuffered */
	setbuf(output, NULL);

	/** Just for debug output = stdout; */

    /** IDs of processes */
	pid_t pid_car = 0, pid_helper = 0, pid_passengers[params.P];

    /** For randomizing */
	srand(time(NULL));

    /** Create car process */
	pid_car = fork();
	if (pid_car == 0) {
		signal(SIGTERM, kill_self);
		signal(SIGINT, kill_self);
		int sleep_time = 0;

		sem_wait(writing);
		fprintf(output, "%d\t: C 1\t: started\n",
			++*actions_counter);
		sem_post(writing);
		for (int i = 0; i < params.P / params.C; i++) {
			sem_wait(writing);
			fprintf(output, "%d\t: C 1\t: load\n",
				++*actions_counter);
			sem_post(writing);
			for (int j = 0; j < params.C; j++) {
				sem_post(boardingQueue);
			}
			sem_wait(allAboard);
			sem_wait(writing);
			fprintf(output, "%d\t: C 1\t: run\n",
				++*actions_counter);
			sem_post(writing);
			if (params.RT) {
                sleep_time = rand() % (params.RT + 1);
                usleep(sleep_time * 1000);
            }
			sem_wait(writing);
			fprintf(output, "%d\t: C 1\t: unload\n",
				++*actions_counter);
			sem_post(writing);
			for (int j = 0; j < params.C; j++) {
				sem_post(unboardingQueue);
			}
			sem_wait(allAshore);
		}
		sem_wait(writing);
		fprintf(output, "%d\t: C 1\t: finished\n",
			++*actions_counter);
		sem_post(writing);
		for (int j = 0; j <= params.P; j++) {
			sem_post(mutex2);
		}

		free_all();

		exit(OK);
	} else if (pid_car > 0) {
	/** in main (parent) process */
	} else {
		fprintf(stderr, "Unable to create process car\n");
		kill_process(pid_helper);
		exit(SYSTEM_CALL_ERROR);
	}

    /** Create helper process */
	pid_helper = fork();

	if (pid_helper == 0) {
		for (int i = 0; i < params.P; i++) {
			signal(SIGTERM, kill_self);
			signal(SIGINT, kill_self);

			int sleep_time = 0;

	    /** Create passanger process */
			pid_passengers[i] = fork();
			if (pid_passengers[i] == 0) {
				signal(SIGTERM, kill_self);
				signal(SIGINT, kill_self);

				int passanger_ID = i + 1;
				sem_wait(writing);
				fprintf(output, "%d\t: P %d\t: started\n",
					++*actions_counter, passanger_ID);
				sem_post(writing);

				sem_wait(boardingQueue);
				sem_wait(writing);
				(*boarders_counter)++;
				fprintf(output, "%d\t: P %d\t: board\n",
					++*actions_counter, passanger_ID);
				if (*boarders_counter == params.C) {
					fprintf(output,
						"%d\t: P %d\t: board last\n",
						++*actions_counter,
						passanger_ID);
					*boarders_counter = 0;
					sem_post(allAboard);
				} else {
					fprintf(output,
						"%d\t: P %d\t: board order %d\n",
						++*actions_counter,
						passanger_ID,
						*boarders_counter);
				}
				sem_post(writing);

				sem_wait(unboardingQueue);
				sem_wait(writing);
				(*unboarders_counter)++;
				fprintf(output, "%d\t: P %d\t: unboard\n",
					++*actions_counter, passanger_ID);
				if (*unboarders_counter == params.C) {
					fprintf(output,
						"%d\t: P %d\t: unboard last\n",
						++*actions_counter,
						passanger_ID);
					sem_post(allAshore);
					*unboarders_counter = 0;
				} else {
					fprintf(output,
						"%d\t: P %d\t: unboard order %d\n",
						++*actions_counter,
						passanger_ID,
						*unboarders_counter);
				}
				sem_post(writing);

				sem_wait(mutex2);
				sem_wait(writing);
				fprintf(output, "%d\t: P %d\t: finished\n",
					++*actions_counter, passanger_ID);
				sem_post(writing);

				free_all();

				exit(OK);
			} else if (pid_passengers[i] > 0) {
		/** in helper process */
			} else {
				fprintf(stderr,
					"Unable to create process passanger\n");
				kill_self();
				exit(SYSTEM_CALL_ERROR);
			}
			if (params.PT) {
                sleep_time = rand() % (params.PT + 1);
                usleep(sleep_time * 1000);
            }
		}

	/** Wait for all passanger processes */
		for (int i = 0; i < params.P; i++) {
			waitpid(pid_passengers[i], NULL, 0);
		}

		free_all();

		exit(OK);

	} else if (pid_helper > 0) {
	/** in main (parent) process */
	} else {
		fprintf(stderr, "Unable to create process helper\n");
		kill_process(pid_car);
		exit(SYSTEM_CALL_ERROR);
	}

	/** Wait for car and helper processes */
	waitpid(pid_car, NULL, 0);
	waitpid(pid_helper, NULL, 0);

    /** Free semaphores, resources and close file */
	free_all();

	return OK;
}
