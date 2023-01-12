#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define SV_IMPLEMENTATION

#define SUDOKU_READ_EMPTY '_'

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define TERMINAL_Y_OFFSET 1
#define TERMINAL_X_OFFSET 2
/* #define CURSOR_ON_SUDOKU(n, y_offset) do {															\ */
/* 		if ((n) % 21<= 2)		set_cursor_position(0 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + (n) % 21 * 15);		\ */
/* 		else if ((n) % 21<= 5)	set_cursor_position(30 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 3) * 15);	\ */
/* 		else if ((n) % 21<= 8)	set_cursor_position(60 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 6) * 15);	\ */
/* 		else if ((n) % 21<= 11)	set_cursor_position(90 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 9) * 15);	\ */
/* 		else if ((n) % 21<= 14)	set_cursor_position(120 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 12) * 15); \ */
/* 		else if ((n) % 21<= 17)	set_cursor_position(150 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 15) * 15); \ */
/* 		else if ((n) % 21<= 20)	set_cursor_position(180 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 18) * 15); \ */
/* 	} while(0) */

/* #define CURSOR_ON_SOLUTIONS_COUNT(n) do {															\ */
/* 		if ((n) % <= 2)		set_cursor_position(0 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + (n) % 21 * 15);				\ */
/* 		else if ((n) % 21<= 5)	set_cursor_position(30 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % 21- 3) * 15);		\ */
/* 		else if ((n) % 21<= 8)	set_cursor_position(60 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % 21- 6) * 15);		\ */
/* 		else if ((n) % 21<= 11)	set_cursor_position(90 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % 21- 9) * 15);		\ */
/* 		else if ((n) % 21<= 14)	set_cursor_position(120 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % 21- 12) * 15);		\ */
/* 		else if ((n) % 21<= 17)	set_cursor_position(150 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % 21- 15) * 15);		\ */
/* 		else if ((n) % 21<= 20)	set_cursor_position(180 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % 21- 18) * 15);		\ */
/* 	} while(0) */

volatile sig_atomic_t interrupted = 0;

double wait_after_guess = -1;
int quiet = 0;
int colors = 0;
int sudokus_vert = 2;
int sudokus_horz = 4;

uint8_t sudoku_beginning[9][9];
uint64_t start_time, end_time;
int solutions = 0;

void set_cursor_position(int x, int y) {
	printf("\033[%d;%dH", y+1, x+1);
}

void disable_cursor() {
	printf("\e[?25l");
}

void enable_cursor() {
	printf("\e[?25h");
}


void cursor_to_solutions_count(int n) {
	int max_amount_solutions = sudokus_horz * sudokus_vert;
	/* if ((n) % max_amount_solutions <= sudokus_vert - 1)		set_cursor_position(0 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + (n) % max_amount_solutions * 15); */
	/* else if ((n) % max_amount_solutions <= 2 * sudokus_vert - 1)	set_cursor_position(30 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % max_amount_solutions- 3) * 15); */
	/* else if ((n) % max_amount_solutions <= 3 * sudokus_vert - 1)	set_cursor_position(60 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % max_amount_solutions- 6) * 15); */
	/* else if ((n) % max_amount_solutions <= 4 * sudokus_vert - 1)	set_cursor_position(90 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % max_amount_solutions- 9) * 15); */
	/* else if ((n) % max_amount_solutions <= 5 * sudokus_vert - 1)	set_cursor_position(120 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % max_amount_solutions- 12) * 15); */
	/* else if ((n) % max_amount_solutions <= 6 * sudokus_vert - 1)	set_cursor_position(150 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % max_amount_solutions- 15) * 15); */
	/* else if ((n) % max_amount_solutions <= 7 * sudokus_vert - 1)	set_cursor_position(180 + TERMINAL_X_OFFSET, 13 + TERMINAL_Y_OFFSET + ((n) % max_amount_solutions- 18) * 15); */
	/* for (size_t i = 0; i < sudokus_horz; i++) { */
	/* 	if (n % max_amount_solutions <= (i+1) * sudokus_vert - 1) { */
	/* 		int x = TERMINAL_X_OFFSET + i * 30; */
	/* 		int y = 13 + TERMINAL_Y_OFFSET + ((n % max_amount_solutions) - (i * sudokus_vert)) * 15; */
	/* 		set_cursor_position(x, y); */
	/* 		return; */
	/* 	} */
	/* } */
	for (int i = 0; i < sudokus_vert; i++) {
		if (n % max_amount_solutions <= (i+1) * sudokus_horz - 1) {
			int x = TERMINAL_X_OFFSET + ((n % max_amount_solutions) % sudokus_horz) * 30;
			int y = TERMINAL_Y_OFFSET + 13 + i * 15;
			set_cursor_position(x, y);
			return;
		}
	}
}

void cursor_to_sudoku(int n, int y_offset) {
	int max_amount_solutions = sudokus_horz * sudokus_vert;
	/* if ((n) % 21<= 2)		set_cursor_position(0 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + (n) % 21 * 15); */
	/* else if ((n) % 21<= 5)	set_cursor_position(30 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 3) * 15); */
	/* else if ((n) % 21<= 8)	set_cursor_position(60 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 6) * 15); */
	/* else if ((n) % 21<= 11)	set_cursor_position(90 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 9) * 15); */
	/* else if ((n) % 21<= 14)	set_cursor_position(120 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 12) * 15); */
	/* else if ((n) % 21<= 17)	set_cursor_position(150 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 15) * 15); */
	/* else if ((n) % 21<= 20)	set_cursor_position(180 + TERMINAL_X_OFFSET, y_offset + TERMINAL_Y_OFFSET + ((n) % 21 - 18) * 15); */
	/* for (size_t i = 0; i < sudokus_horz; i++) { */
	/* 	if (n % max_amount_solutions <= (i+1) * sudokus_vert - 1) { */
	/* 		int x = TERMINAL_X_OFFSET + i * 30; */
	/* 		int y = TERMINAL_Y_OFFSET + y_offset + ((n % max_amount_solutions) - (i * sudokus_vert)) * 15; */
	/* 		set_cursor_position(x, y); */
	/* 		return; */
	/* 	} */
	/* } */
	for (int i = 0; i < sudokus_vert; i++) {
		if (n % max_amount_solutions <= (i+1) * sudokus_horz - 1) {
			/* int x = TERMINAL_X_OFFSET + i * 30; */
			/* int y = TERMINAL_Y_OFFSET + y_offset + ((n % max_amount_solutions) - (i * sudokus_horz)) * 15; */
			int x = TERMINAL_X_OFFSET + ((n % max_amount_solutions) - (i * sudokus_horz)) * 30;
			int y = TERMINAL_Y_OFFSET + y_offset + i * 15;
			set_cursor_position(x, y);
			return;
		}
	}
}

// used right before exiting the program to hide the unused working sudoku which was displayed to the user
void hide_unsolved_sudokus() {
	/* if (solutions > 2) CURSOR_ON_SOLUTIONS_COUNT(2); */
	if (solutions >= sudokus_horz * sudokus_vert) cursor_to_solutions_count(sudokus_horz * sudokus_vert - 1);
	/* else if (solutions == 0) CURSOR_ON_SOLUTIONS_COUNT(0); */
	else cursor_to_solutions_count(solutions);
	/* else CURSOR_ON_SOLUTIONS_COUNT(solutions - 1); */
	/* else cursor_to_solutions_count(solutions - 1); */
	printf("\n");
}

void read_sudoku(uint8_t sudoku[9][9], char buf[9][9]) {
	for (size_t i = 0; i < 9; i++) {
		for (size_t j = 0; j < 9; j++) {
			sudoku[i][j] = buf[i][j] != SUDOKU_READ_EMPTY ? buf[i][j] - '0' : 0;
		}
	}
}

ssize_t next_free_index(uint8_t sudoku[9][9]) {
	for (size_t i = 0; i < 9; i++) {
		for (size_t j = 0; j < 9; j++) {
			if (sudoku[i][j] == 0) {
				return i * 9 + j;
			}
		}
	}
	/* set_cursor_position(0, 17); */
	/* printf("ERROR: did not find free index\n"); */
	return -1;
}

int solved(uint8_t sudoku[9][9]) {
	// Check rows and columns simultanously
	for (size_t i = 0; i < 9; i++) {
		uint8_t sprite_row[9] = {0};
		uint8_t sprite_col[9] = {0};
		for (size_t j = 0; j < 9; j++) {
			uint8_t entry_row = sudoku[i][j];
			uint8_t entry_col = sudoku[j][i];
			if (entry_col == 0 || entry_row == 0) return 0;
			if (sprite_row[entry_row-1] == 1) return 0;
			if (sprite_col[entry_col-1] == 1) return 0;
			sprite_row[entry_row-1] = 1;
			sprite_col[entry_col-1] = 1;
		}

		// now we check the 9 3x3 squares for validity
		uint8_t sprite_block[9] = {0};
		size_t block_start = i / 3 * 3; // top left entry of one squre
		// block_start varies for the 9 squares:
		// i = 0:  block_start = 0
		// i = 1:  block_start = 3
		// i = 2:  block_start = 6
		// i = 3:  block_start = 27
		// i = 4:  block_start = 30
		// i = 5:  block_start = 33
		// i = 6:  block_start = 54
		// i = 7:  block_start = 57
		// i = 8:  block_start = 60
		if (i <= 5) block_start += 27;
		if (i <= 8) block_start += 27;
		
		for (size_t j = 0; j < 3; j++) {  // row offset from block_start
			for (size_t k = 0; k < 3; k++) {  // column offset from block_start
				size_t index = block_start + j * 9 + k;
				uint8_t entry = sudoku[index/9][index%9];
				if (entry == 0) return 0;

				if (sprite_block[entry-1] == 1) return 0;
				sprite_block[entry-1] = 1;
			}
		}
	}
	// all checks passed -> valid soduku
	return 1;
}

void dump_sudoku(uint8_t sudoku[9][9], int n) {
	/* CURSOR_ON_SUDOKU(n, 0); */
	cursor_to_sudoku(n, 0);
	for (size_t row = 0; row < 9; row++) {
		/* CURSOR_ON_SUDOKU(n, row + row/3); */
		cursor_to_sudoku(n, row + row/3);
		if (row % 3 == 0) printf("=========================\n");
		/* CURSOR_ON_SUDOKU(n, row + row/3 + 1); */
		cursor_to_sudoku(n, row + row/3 + 1);
		for (size_t col = 0; col < 9; col++) {
			if (col == 0) printf("|");
			else if (col % 3 == 0) printf(" |");
			printf(" ");
			// Fix entry, color red
			if (colors && sudoku_beginning[row][col]) printf(ANSI_COLOR_RED);
			// non zero entry, color green
			else if (colors && sudoku[row][col]) printf(ANSI_COLOR_GREEN);
			printf("%d", sudoku[row][col]);
			if (colors && sudoku[row][col]) printf(ANSI_COLOR_RESET);
		}
		printf(" |");
	}
	/* CURSOR_ON_SUDOKU(n, 12); */
	cursor_to_sudoku(n, 12);
	printf("=========================\n");
}

int is_possible_entry(uint8_t sudoku[9][9], ssize_t index, size_t n) {
	assert(0 <= index && index <= 81);
	assert(1 <= n && n <= 10);
	
	size_t row = index / 9;
	size_t col = index % 9;

	uint8_t row_entries[9] = {0};
	uint8_t col_entries[9] = {0};
	for (size_t i = 0; i < 9; i++) {
		uint8_t entry_row = sudoku[row][i];
		uint8_t entry_col = sudoku[i][col];
		
		if (i != col && entry_row > 0) { // skip index itself in row
			if (row_entries[entry_row-1] == 1) {
				assert(0 && "unreachable");
			}
			row_entries[entry_row-1] = 1;
		}
		if (i != row && entry_col > 0) { // skip index itself in column
			if (col_entries[entry_col-1] == 1) {
				assert(0 && "unreachable");
			}
			col_entries[entry_col-1] = 1;
		}
	}

	if (row_entries[n-1] == 1 || col_entries[n-1] == 1) return 0;

	uint8_t block_entries[9] = {0};
	size_t block_start = col / 3 * 3;
	if (row >= 3) block_start += 27;
	if (row >= 6) block_start += 27;
	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++) {
			ssize_t current_index = block_start + i * 9 + j;
			if (current_index == index) continue;  // skip index itself
			uint8_t entry = sudoku[current_index/9][current_index%9];
			if (entry == 0) continue;
			
			if (block_entries[entry-1] == 1) {
				assert(0 && "unreachable");
			}
			block_entries[entry-1] = 1;
		}
	}

	return block_entries[n-1] == 0;
}

void kill_prog(uint8_t sudoku[9][9]) {
	(void) sudoku;
	cursor_to_solutions_count(solutions - 1);
	printf("Solutions (aborted): %d\n", solutions);
	hide_unsolved_sudokus();
	enable_cursor();
	exit(1);
}

int dfs(uint8_t sudoku[9][9]) {
	if (interrupted) {
		kill_prog(sudoku);
	}
	if (solved(sudoku)) {
		solutions++;
		if (quiet) {
			/* CURSOR_ON_SUDOKU(solutions - 1, 0); */
			cursor_to_sudoku(solutions - 1, 0);
			dump_sudoku(sudoku, solutions - 1);
		}
		/* CURSOR_ON_SOLUTIONS_COUNT(solutions - 1); */
		cursor_to_solutions_count(solutions - 1);
		printf("Solution: %d\n", solutions);
		if (solutions >= sudokus_vert * sudokus_horz && sudokus_vert * sudokus_horz > 1) {
			/* CURSOR_ON_SOLUTIONS_COUNT(solutions); */
			cursor_to_solutions_count(solutions);
			printf("                  \n");
		}
		return 1;
	}
	ssize_t index = next_free_index(sudoku);
	if (index < 0) return 0;
	for (size_t i = 1; i <= 9; i++) {
		if (!is_possible_entry(sudoku, index, i)) continue;
		start_time = time(NULL);
		sudoku[index/9][index%9] = i;
		if (!quiet) {
			/* CURSOR_ON_SUDOKU(solutions, 0); */
			cursor_to_sudoku(solutions, 0);
			dump_sudoku(sudoku, solutions);
		}
		end_time = time(NULL);
		
		if (wait_after_guess - (end_time - start_time) > 0.05) {
			usleep(wait_after_guess - (end_time - start_time));
		}
		dfs(sudoku);
	}
	// sudoku is wrong in any previous guessed entry, reset this entry
	sudoku[index/9][index%9] = 0;
	return 0;
}

void usage(char *prog) {
	fprintf(stderr, "Usage: %s [opts] <sudoku.txt>\n", prog);
	fprintf(stderr, "    Options:\n");
	fprintf(stderr, "        -g <guesses_per_second>  specifies the number of guesses per second. Cool for visualization\n");
	fprintf(stderr, "        -q                       quiet mode. Does not show the actual work on the sudoku\n");
	fprintf(stderr, "        -c                       colorful output\n");
	fprintf(stderr, "        -h                       amount columns of solutions displayed\n");
	fprintf(stderr, "        -v                       amount rows of solutions displayed\n");
}

void sigint_handler(int sig) {
	(void) sig;
	interrupted = 1;
}

int main(int argc, char **argv)
{
	// read optional arguments
	int opt;
	while((opt = getopt(argc, argv, "qcg:h:v:")) != -1) {
		switch(opt) {
		case 'c':
			colors = 1;
			break;
		case 'q':
			quiet = 1;
			break;
		case 'g':
			wait_after_guess = 1000000/atof(optarg);
			break;
		case 'h':
			sudokus_horz = atoi(optarg);
			if (sudokus_horz <= 0) {
				usage(argv[0]);
				exit(1);
			}
			break;
		case 'v':
			sudokus_vert = atoi(optarg);
			if (sudokus_vert <= 0) {
				usage(argv[0]);
				exit(1);
			}
			break;
		default:
			usage(argv[0]);
			return -1;
		}
	}

	// we need two more args for src and dest
	if (optind != argc - 1) {
		fprintf(stderr, "ERROR: Expected sudoku filepath after options\n");
		usage(argv[0]);
		return -1;
	}

	char *sudoku_file = argv[optind];

	FILE *fp = fopen(sudoku_file, "r");
	if (fp == NULL) exit(1);
	
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	if (size != 82) {
		fprintf(stderr, "ERROR: invalid sudoku file, got %ld entries, need 81 (9x9)\n", size);
		exit(1);
	}
	fseek(fp, 0, SEEK_SET);
	
	char buf[9][9];
	if (buf == NULL) exit(1);
	
	if (fread(buf, 81, 1, fp) != 1) {
		fprintf(stderr, "ERROR: did not read all bytes from %s\n", sudoku_file);
		exit(1);
	}
	fclose(fp);

	uint8_t sudoku[9][9];
	read_sudoku(sudoku, buf);
	for (size_t i = 0; i < 9; i++) {
		for (size_t j = 0; j < 9; j++) {
			sudoku_beginning[i][j] = sudoku[i][j];
		}
	}

	if (!quiet) {
		set_cursor_position(0, 15);
	} else if (quiet) {
		set_cursor_position(0, 2);
	}
	printf("Solutions: %d", solutions);
	fflush(stdout);

	// terminal related stuff
	signal(SIGINT, sigint_handler); // enable cursor before sigint so we have our cursor back
	printf("\e[1;1H\e[2J"); // clear terminal screen (demands POSIX)
	disable_cursor();
	printf("\n");

	dfs(sudoku);

	// 'delete' last sudoku field which holds an invalid solution if quiet flag is not set
	hide_unsolved_sudokus();
	
	enable_cursor();
	return 0;
}
