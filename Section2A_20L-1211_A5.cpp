#include<iostream>
#include<fstream>
#include<ctype.h>
#include<sstream>
#include<cstring>
#include<string>

using namespace std;
class GridGenerator {

private:
	//String that will have all the words that exists in the file in appended style
	string words = "";
	//Integer to hold maximum length of a word
	int max_length = 0;
	//integer to count how many words have been read from file
	int words_count = 0;
	//Character Board that will hold all the crossword-grid
	char **board;
	//Dynamic Character Array that will hold all the words
	char **board_word;

	//Gloabl varibales
	int row = 0, col = 0;
	int No_Rows, No_Cols;


public:

	int GetNo_Rows()
	{
		return No_Rows;
	}
	int GetNo_Cols()
	{
		return No_Cols;
	}
	int Getmax_length()
	{
		return max_length;
	}

	//Function To read all the words from file
	bool read_from_file() {

		//Creating file reading object	

		ifstream fin;
		string filename;
		cout << "ENTER THE INPUT FILE NAME WITH EXTENSION " << endl;
		cin >> filename;
		cout << endl;
		cout << "NUMBER OF ROWS IN THE GRID " << endl;
		cin >> No_Rows;
		cout << endl;
		cout << "NUMBER OF COLUMNS IN THE GRID " << endl;
		cin >> No_Cols;
		cout << endl;


		//Opening file
		fin.open(filename);


		//If file is opened
		if (fin.is_open()) {

			bool check = true;
			string temp;

			//This loop will read all the words from file
			while (!fin.eof()) {

				fin >> temp;

				//Checking if the temporary word read,is a word or not
				//if it is not,flag wil be changed to false and loop will be exited
				for (int i = 0; i < temp.length(); i++) {
					if (isalpha(temp[i]) == 0) {
						check = false;
						break;
					}
				}

				//if it is,
				if (check) {

					//Updating maximum length if possible
					if (temp.length() > max_length) {
						max_length = temp.length();

					}

					//else appending it to the string
					words += temp + "\n";

					//Increasing total word count
					words_count++;


				}
				else {
					//else changing flag back to false
					check = false;
				}

			}


		}
		//if input file can't be found
		else {

			cout << "Can't Find Input File";
			fin.close();
			return false;

		}
		//closing file
		fin.close();
		//returning true
		return true;


	}

	//Function that will assign board the words
	void assign_board() {


		board = new char*[No_Rows];
		board_word = new char*[No_Rows];

		for (int i = 0; i < No_Rows; i++) {

			// Declare a memory block
			// of size n
			board[i] = new char[No_Cols];

			for (int j = 0;j < No_Cols;j++) {
				board[i][j] = '#';
			}
			board[i][No_Cols] = '\0';
		}


		simple_tokenizer();
		solve();
		file_board_word();

	}

	//Function to tokenize all the words read from file to store

	void simple_tokenizer()
	{
		stringstream ss(words);
		string word;
		int count = 0;
		int size = No_Cols;
		while (ss >> word) {

			board_word[count] = new char[word.size() + 1];
			for (int i = 0;i < word.size();i++) {
				board_word[count][i] = tolower(word[i]);
			}
			board_word[count][word.size()] = '\0';
			count++;
		}

		sort(board_word, words_count);
	}

	//Function to print board
	void file_board_word() {



		ofstream fout;
		string outputfile;
		cout << "ENTER THE OUTPUT FILE NAME WITH EXTENSION  " << endl;
		cin >> outputfile;
		cout << endl;
		fout.open(outputfile);
		for (int i = 0;i < No_Rows;i++) {

			for (int j = 0;j < No_Cols;j++) {

				if (board[i][j] == '#' || (board[i][j] != '#' && !(board[i][j] >= 'a' && board[i][j] <= 'z'))) {

					char cch = 'a' + rand() % 26;
					fout << cch << " ";
				}
				else {

					fout << board[i][j] << " ";
				}
			}
			fout << "\n";

		}
		fout.close();

	}

	//Sort the 2d-Character Arrayt ascendingly
	void sort(char** s, int n)
	{
		for (int i = 1;i < n; i++)
		{
			string temp = s[i];

			// Insert s[j] at its correct position
			int j = i - 1;
			while (j >= 0 && temp.length() < strlen(s[j]))
			{
				s[j + 1] = s[j];
				j--;
			}
			for (int k = 0;k < temp.length();k++) {
				s[j + 1][k] = temp[k];
			}
		}
	}

	//Function To Solve and place word accordingly if condition matches

	void solve() {

		for (int i = words_count - 1; i > 0; ) {

			if (i == (words_count - 1)) {
				place_word_diagnally(0, board_word[i], strlen(board_word[i]));
				i--;
			}
			else {

				int j = 0;

				if (strlen(board_word[i]) < this->words_count / 2) {
					for (j = 0; j < No_Rows; j++) {

						if (check_vertically(0, j, board_word[i], strlen(board_word[i]))) {
							place_vertically(this->row, this->col, board_word[i], strlen(board_word[i]));
							i--;
							break;
						}

					}
				}

				for (j = 0;j < words_count;j++) {



					if (check_horizontally(j, 0, board_word[i], strlen(board_word[i]))) {
						place_horizontally(this->row, this->col, board_word[i], strlen(board_word[i]));
						i--;
						break;
					}

				}

				if (j == 10) {

					for (j = 0; j < words_count; j++) {

						if (check_vertically(0, j, board_word[i], strlen(board_word[i]))) {
							place_vertically(this->row, this->col, board_word[i], strlen(board_word[i]));
							i--;
							break;
						}

					}
				}
				if (j == 10) {
					i--;
				}
			}


		}

	}

	//Function that will place word horizontally,specified the row and column
	void place_horizontally(int row, int col, char* word, int len) {
		int column = col, counter = 0;
		for (column, counter; column < len;column++, counter++) {
			board[row][column] = word[counter];
		}

	}

	//Function that will place word vertically,specified the row and column
	void place_vertically(int row, int col, char* word, int len) {

		for (int r = row, counter = 0; counter < len; r++, counter++) {
			board[r][col] = word[counter];
		}
	}

	void place_word_diagnally(int row, char* word, int len) {


		for (int i = row; i < len;i++) {
			board[i][i] = word[i];
		}

	}

	//Function that will check whether if the word could be placed horizontally
	//defined the row and column
	bool check_horizontally(int row, int column, char* board_word, int len) {


		bool flag = true;

		if (row < words_count && column < max_length) {

			for (int col = column, counter = 0; col < len;col++, counter++) {
				if (col < max_length && col < words_count) {

					if (board[row][col] != '#') {
						if (board[row][col] != board_word[counter]) {
							flag = false;
							break;
						}
					}

				}
				else {
					flag = false;
					break;
				}
			}

		}
		if (flag == false) {

			if ((max_length - (column + 1)) > len) {

				check_horizontally(row, column + 1, board_word, len);
			}
			return false;
		}

		if (flag == true) {
			this->row = row;
			this->col = column;
		}

		return flag;

	}

	//Function that will check whether if the word could be placed vertically
	//defined the row and column
	bool check_vertically(int row, int column, char* board_word, int len) {

		bool flag = true;

		if (row < words_count && column < max_length) {

			for (int r = row, counter = 0; counter < len; r++, counter++) {
				if (r < max_length && r < words_count) {

					if (board[r][column] != '#') {
						if (board[r][column] != board_word[counter]) {
							flag = false;
							break;
						}
					}

				}
				else {
					flag = false;
					break;
				}
			}

		}
		if (flag == false) {

			if ((words_count - (row + 1)) > len) {

				flag = check_vertically(row + 1, column, board_word, len);
			}
			return flag;
		}

		if (flag == true) {
			this->row = row;
			this->col = column;

		}

		return true;


	}


};



class WordSearch
{
	char **arr;//puzzle array
	int rows = 0, columns = 0, x_pos = 0, y_pos = 0, word_length = 0, n_checker = 0;
	int max_length = 0;
	int ending_row = 0, ending_col = 0;



public:

	void takeinp()
	{
		ifstream obj;

		string filename;
		cout << "PLEASE ENTER THE INPUT FILE NAME  WITH EXTENSION " << endl;
		cin >> filename;
		cout << endl;
		obj.open(filename);

		if (obj.is_open())
		{
			int count = 0;
			string temp;
			while (!obj.eof()) {
				obj >> temp;
				columns = temp.size();
				count++;
			}
			rows = count - 1;
			obj.close();
			obj.open(filename);

			arr = new char*[rows];

			for (int i = 0;i < rows;i++)
			{
				arr[i] = new char[columns];
			}

			for (int i = 0;i < rows;i++)//inputting character of puzzle one by one
			{
				obj >> arr[i];
			}

		}
		else
		{
			cout << "\nCAN'T FIND FILE";
		}

		obj.close();
	}

	void Solve(string word, string File)
	{
		int k_flag = 0;
		int initial = 0;

		//oup.open(Filename, ios::app);
		ofstream oup(File.c_str(), ios::app);

		if (n_checker == 0 && initial != 0)//if word not found simply writing "not found" in file
		{
			oup << " " << "NOT FOUND" << "\n";
			cout << " " "NOT FOUND" << "\n";
		}
		else
		{
			n_checker = 0;
		}
		k_flag = 0;
		word_length = word.length();

		for (int j = 0;j < rows;j++)//iterating by row
		{
			if (k_flag != 1)//if word not found at previous k iteration then forward 
			//onto next k iteration(it will run 1 time as k is initially 0)
			{


				for (int k = 0;k < columns;k++)//iterating by column
				{
					if (word[0] == arr[j][k])
					{

						if (k + word_length <= columns)//boundary check
						{
							if (right(word, j, k, word_length))
							{

								oup << "  " << word << "  { " << j << "," << k << " }" << ", { " << ending_row << "," << ending_col << " }" << "\n";
								cout << "  " << word << " { " << j << "," << k << " }" << ", { " << ending_row << "," << ending_col << " }" << "\n";
								k_flag = 1;
								n_checker = 1;
								initial = 1;
								break;

							}
						}

						if (columns - word_length >= 0)//boundary check
						{

							if (left(word, j, k, word_length))//i=word[i],j=loop_row,k=loop_col
							{

								oup << "  " << word << " { " << j << "," << k << " }" << ", { " << ending_row << "," << ending_col << " }" << "\n";
								cout << "  " << word << "{ " << j << "," << k << " }" << ", { " << ending_row << "," << ending_col << " }" << "\n";
								k_flag = 1;
								n_checker = 1;
								initial = 1;
								break;

							}
						}

						if (j + word_length <= rows)//boundary check
						{
							if (down(word, j, k, word_length))//i=word[i],j=loop_row,k=loop_col,columns=col_limit
							{

								oup << "  " << word << " { " << j << "," << k << " }" << ", { " << ending_row << "," << ending_col << " }" << "\n";
								cout << "  " << word << "{ " << j << "," << k << " }" << ", { " << ending_row << "," << ending_col << " }" << "\n";
								k_flag = 1;
								n_checker = 1;
								initial = 1;
								break;

							}
						}

						if (word_length - j <= rows)//boundary check
						{
							if (up(word, j, k, word_length))//i=word[i],j=loop_row,k=loop_col,columns=col_limit
							{

								oup << "  " << word << " { " << j << "," << k << " }" << ", { " << ending_row << "," << ending_col << " }" << "\n";
								cout << "  " << word << " { " << j << "," << k << " }" << ", { " << ending_row << "," << ending_col << " }" << "\n";
								k_flag = 1;
								n_checker = 1;
								initial = 1;
								break;

							}
						}

						if ((j >= 0 && j < rows) && (k < columns && k >= 0)) {

							if (diagonal(word, j, k, word_length)) {

								oup << "  " << word << " { " << j << "," << k << " }" << ", {" << ending_row << "," << ending_col << " }" << "\n";
								cout << " " << word << "{ " << j << "," << k << " }" << ", { " << ending_row << "," << ending_col << " }" << "\n";
								k_flag = 1;
								n_checker = 1;
								initial = 1;
								break;
							}

						}

					}
				}

			}

			else//if word is found then simply break and increase i
			{
				break;
			}


		}
		if (initial == 0) {
			oup << "\nWord " << word << " NOT FOUND\n";
			cout << "\nWord " << word << " NOT FOUND\n";
		}

		oup.close();

	}




	//functions for searching

	int diagonal(string word, int row, int col, int length) {

		int rt = row;
		int rc = col;
		bool flag = true;

		if (((row + length - 1) < rows) && ((col + length - 1) < columns)) // checking for right diagonal
		{
			for (int i = 0; i < length; i++) {

				if (word[i] != arr[row][col]) {

					flag = false;
					break;
				}
				row++;
				col++;

			}

			if (flag == true) {

				ending_row = row - 1;
				ending_col = col - 1;
				return 1;
			}
		}

		row = rt;
		col = rc;
		flag = true;

		if (((row + length - 1) < rows) && (col - (length - 1)) >= 0) // checking for left diagonal
		{
			for (int i = 0; i < length; i++) {

				if (word[i] != arr[row][col]) {

					flag = false;
					break;
				}
				row++;
				col--;

			}

			if (flag == true) {

				ending_row = row - 1;
				ending_col = col + 1;
				return 1;

			}
		}
		row = rt;
		col = rc;
		flag = true;
		if ((row >= length - 1) && (col >= length - 1)) // checking for invert left diagonal
		{
			for (int i = 0; i < length; i++) {

				if (word[i] != arr[row][col]) {

					flag = false;
					break;
				}
				row--;
				col--;

			}

			if (flag == true) {

				ending_row = row + 1;
				ending_col = col + 1;
				return 1;

			}
		}
		row = rt;
		col = rc;
		flag = true;
		if (((row - length - 1) >= 0) && ((col + length) < columns)) // checking for invert right diagonal
		{
			for (int i = 0; i < length; i++) {

				if (word[i] != arr[row][col]) {

					flag = false;
					break;
				}
				row--;
				col++;

			}

			if (flag == true) {

				ending_row = row + 1;
				ending_col = col - 1;
				return 1;

			}
		}
		return 0;

	}


	int up(string word, int row, int col, int length)
	{
		int base = 0;
		char *temp_u = new char[length + 1];
		memset(temp_u, 0, length);


		int i = 0;

		while (i != length && row >= 0)
		{


			temp_u[base] = arr[row][col];
			base += 1;
			--row;
			++i;
		}
		row += 1;
		temp_u[length] = '\0';


		if (strcmp(temp_u, word.c_str()) == 0)
		{
			delete[] temp_u;
			temp_u = NULL;
			ending_row = row;
			ending_col = col;
			row = 0;
			col = 0;
			return 1;

		}
		else
		{
			delete[] temp_u;
			temp_u = NULL;
			row = 0;
			col = 0;
			return 0;
		}

	}
	int down(string word, int row, int col, int length)
	{
		int base = 0;
		char *temp_d = new char[length + 1];
		memset(temp_d, 0, length);


		int i = 0;


		while (i != length)
		{

			temp_d[base] = arr[row][col];
			base += 1;
			++row;
			++i;
		}
		temp_d[length] = '\0';
		row -= 1;

		if (strcmp(temp_d, word.c_str()) == 0)
		{
			delete[] temp_d;
			temp_d = NULL;
			ending_row = row;
			ending_col = col;
			row = 0;
			col = 0;
			return 1;

		}
		else
		{
			delete[] temp_d;
			temp_d = NULL;
			row = 0;
			col = 0;
			return 0;
		}


	}
	int left(string word, int row, int col, int length)
	{
		int base = 0;
		char *temp_l = new char[length + 1];
		memset(temp_l, 0, length);


		int i = 0;

		while (i != length)
		{

			temp_l[base] = arr[row][col];
			base += 1;
			--col;
			++i;

		}

		temp_l[base] = '\0';

		if (strcmp(temp_l, word.c_str()) == 0)
		{
			delete[] temp_l;
			temp_l = NULL;
			if (row < 0) {
				row = 0;
			}
			if (col < 0) {
				col = 0;
			}
			ending_row = row;
			ending_col = col;
			row = 0;
			col = 0;
			return 1;

		}
		else
		{
			delete[] temp_l;
			temp_l = NULL;
			row = 0;
			col = 0;
			return 0;
		}
	}
	int right(string word, int row, int col, int length)
	{
		int base = 0, flag = 0;
		char *temp_r = new char[length + 1];
		memset(temp_r, 0, length);


		int i = 0;

		while (i != length)
		{


			temp_r[base] = arr[row][col];
			base += 1;
			++col;
			++i;

		}
		temp_r[length] = '\0';
		col = col - 1;

		if (strcmp(temp_r, word.c_str()) == 0)
		{
			delete[] temp_r;
			temp_r = NULL;
			ending_row = row;
			ending_col = col;
			col = 0;
			row = 0;
			return 1;
		}
		else
		{
			delete[] temp_r;
			temp_r = NULL;
			col = 0;
			row = 0;
			return 0;
		}

	}
};




int main()
{

	char option;
	do
	{
		cout << endl << endl;
		cout << "\t\t ----------------------------------------------------------------------------" << endl;
		cout << "\t\t                               WORD SEARCH GAME                           " << endl;

		cout << "\t\t  ---------------------------------------------------------------------------" << endl;
		cout << endl;
		cout << "\tENTER C. TO CREATE THE GRID " << endl;
		cout << "\tENTER S. TO SEARCH IN THE GRID " << endl;
		cout << "\tENTER Q. TO QUIT  " << endl;
		cin >> option;
		option = toupper(option);


		switch (option)
		{
		case 'C':
		{
			GridGenerator obj;
			if (obj.read_from_file()) {
				if (obj.Getmax_length() > obj.GetNo_Rows() || obj.Getmax_length() > obj.GetNo_Cols())
				{
					cout << endl;
					cout << "\t GRID CAN NOT FORMED " << endl;
					cout << endl;
					break;
				}
				else
				{
					cout << endl;
					cout << "\t GRID FORMED  SUCCESSFULLY " << endl;
					cout << endl;
				}
				obj.assign_board();
			}
			cout << "PRESS ANY KEY TO RETURN TO THE HOME SCREEN  " << endl;
			break;
		}
		// Read words from a  file that needs to be searched and then displat their location 
		// on console as well as in the output file 
		case'S':
		{
			WordSearch obj;
			obj.takeinp();//taking input from file
			ifstream object;
			string filename;
			string temp;
			int no_ofwords;
			string Filename;
			cout << "ENTER THE OUTPUT FILE NAME WITH EXTENSION" << endl;
			cin >> Filename;
			ofstream oup(Filename, ios::app);
			if (oup.is_open())
			{
			}
			else
			{
				cout << "\nCAN'T OPEN FILE";
				exit(0);
			}

			cout << "ENTER THE FILE WITH WORDS TO BE SEARCHED WITH EXTENSION" << endl;
			cin >> filename;
			cout << endl;
			object.open(filename);
			if (object.is_open())
			{
				object >> no_ofwords;               //  number of search words in the file 
				for (int i = 0;i < no_ofwords;i++)
				{
					object >> temp;
					obj.Solve(temp, Filename);
				}
			}
			cout << "PRESS ANY KEY TO RETURN TO THE HOME SCREEN  " << endl;
			break;
		}
		case'Q':
		{
			cout << "GAME ENDED " << endl;
			return 0;
			break;
		}
		default:
		{
			cout << "CHOOSE A CORRECT OPTION" << endl;
			break;
		}

		}

	} while (option != 'Q');
}