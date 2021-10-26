//You can include any of headers defined in the C11 standard here. They are:
//assert.h, complex.h, ctype.h, errno.h, fenv.h, float.h, inttypes.h, iso646.h, limits.h, locale.h, math.h, setjmp.h, signal.h, stdalign.h, stdarg.h, stdatomic.h, stdbool.h, stddef.h, stdint.h, stdio.h, stdlib.h, stdnoreturn.h, string.h, tgmath.h, threads.h, time.h, uchar.h, wchar.h or wctype.h
//You may not include any other headers.
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include<errno.h>
#include"connect4.h"


struct board_structure {
  int num_rows; // num_rows stores the number of rows
  int num_cols; // num_cols stores the number of columns
  int num_x; // num_x stores number of x's on the board
  int num_o; // num_o stores the number of o's on the board
  int min_row; // Used in is_winning_move  for less runtime
  char current_winner; // stores current winner(linked to the current_winner function)
  char token; // stores x or o
  char *mat; // data structure to store the board, it is actually a 1D array which can be accessed like a 2d array or matrix
};

board setup_board(){
struct board_structure *ptr;
ptr=(struct board_structure *) malloc( sizeof(struct board_structure)); // dynamically allocating memory for the board_structure struct
ptr->num_cols=1;
ptr->num_rows=0;
ptr->num_x=0;
ptr->num_o=0;
ptr->min_row=0;
ptr->token='.';
ptr->current_winner='.';
return ptr;
}

void cleanup_board(board u){
  free(u->mat);
  free(u);
}

void read_in_file(FILE *infile, board u){

  int i=0;int j=0;int l=0;int k=0;char c; // variables used in loops and for making checks

  
  for (c = getc(infile); c != EOF; c = getc(infile)){ // calculates length of first line in infile and stores it in l
    l++;
    if (c == '\n'){
      break;
    }
  }

  rewind(infile);

  for (c = getc(infile); c != EOF; c = getc(infile)){ // traverses through the file and calculates number of rows,columns,x's,o's
    u->num_cols=u->num_cols+1;
    k+=1;
    if (c == '.'){
      continue;}

    else if (c == '\n'){
      if (l!=k){ // compares current line length with first line 
        printf("The file is invalid as the rows and/or columns are uneven %d %d \n",k,l);
        u->current_winner = 'E';
        return;}
      k=0;
      u->num_rows=u->num_rows+1;}

    else if (c == 'x'){
      u->num_x=u->num_x+1;}

    else if (c == 'o'){
      u->num_o=u->num_o+1;}
    
    else {
      u->current_winner = 'E';
      printf("The file contains an invalid character(s) such as '%c' \n",c);
      return;
    }
  }

  if (l!=k && k!=0){ // for the last line
    printf("The file is invalid as the rows and/or columns are uneven %d %d \n",k,l);
    u->current_winner = 'E';
    return;}

  if (u->num_x-u->num_o>1){ // if there are 2 or more x's than o's
    u->current_winner = 'E';
    printf("The file is invalid because there are %d more x's than o's \n",u->num_x-u->num_o);
    return;}

  else if (u->num_o-u->num_x>0){ // if there are 1 or more o's than x's
    u->current_winner = 'E';
    printf("The file is invalid because there are %d more o's than x's \n",u->num_o-u->num_x);
    return;}

  if (u->num_cols>512){ // file can't have more than 512 columns
    printf("The file is invalid as there are more than 512 columns \n");
    u->current_winner = 'E';
    return;
  }
  else if(u->num_cols<4){ //  file can't have less than 4 columns
    printf("The file is invalid as there are less than 4 columns \n");
    u->current_winner = 'E';
    return;
  }
  else if(u->num_rows<4){ //  file can't have less than 4 rows
    printf("The file is invalid as there are less than 4 rows \n");
    u->current_winner = 'E';
    return;
  }

  u->num_cols=(u->num_cols-u->num_rows)/u->num_rows;

  u->mat = (char *)malloc(u->num_rows * u->num_cols * sizeof(char));
  // allocating memory for the matrix of the board based on the number of columns and rows

  rewind(infile);

  for (c = getc(infile); c != EOF; c = getc(infile)){ // stores the board in u->mat
    if (c == '\n'){
      j=0;
      i+=1;
      continue;
    }
    else if(c=='o'||c=='x'||c=='.') {
      *(u->mat + i*u->num_cols + j) = c;
      j+=1;
    }
  }

  char temp;
  
  for (i=u->num_rows;i>-1;i--){ // gravity is applied to the board so as to prevent any errors
    for (j=0;j<u->num_cols;j++){
      temp=*(u->mat + i*u->num_cols + j);
      k=i;l=1;
      if (temp == 'x' || temp == 'o'){
        *(u->mat + i*u->num_cols + j)='.';
        while(l){
          if (*(u->mat + (k+1)*u->num_cols + (j)) == '.'){
            k++;
          }
          else {
            if (u->min_row<k){
              u->min_row=k;
            }
            *(u->mat + (k)*u->num_cols + (j)) = temp;
            l=0;
          }
        }
      }
    }
  }
  
  printf("The number of x's is %d\n",u->num_x);
  printf("The number of o's is %d\n",u->num_o);
  printf("The number of columns is %d\n",u->num_cols);
  printf("The number of rows is %d\n",u->num_rows);

}

void write_out_file(FILE *outfile, board u){
  if (u->current_winner == 'E'){ // E means there is an error
    printf("Board was not written into the outfile as the board is invalid \n");
    return;}

  for (int i=0;i<u->num_rows;i++){ // prints board into the outfile
    for (int j=0;j<u->num_cols;j++){
      fprintf(outfile,"%c",*(u->mat + i*u->num_cols + j));
    }
    fprintf(outfile,"\n");
  }
}

char next_player(board u){
  if (u->num_x>u->num_o){ // if x's > o's o plays next
    return 'o';
  }
  else { // if o's = x's x plays next
    return 'x';
  }
}

char current_winner(board u){
  if (u->current_winner == 'E'){ // E means there is an error
      return u->current_winner;
  }
  return u->current_winner;
}

int clean_stdin();

struct move read_in_move(board u){
  struct move a_move;
  char c;
  do{ // prevents user from entering non-numerical values
  printf("Player %c enter column to place your token: ",next_player(u));
  }
  while (((scanf("%d%c", &a_move.column, &c)!=2 || c!='\n') && clean_stdin()));

  do{ // prevents user from entering non-numerical values
    printf("Player %c enter row to rotate: ",next_player(u));
  }
  while (((scanf("%d%c", &a_move.row, &c)!=2 || c!='\n') && clean_stdin()));

  return a_move; // returns struct a_move which documents the move
}

int is_valid_move(struct move m, board u){
  // u->current_winner='x';
  if (u->num_cols < m.column){
    printf("You have tried to place a token at column %d but there are only %d columns\n",m.column,u->num_cols);
    return 0;
  }
  else if (m.column<1){
    printf("You have tried to place a token at a column below 1, please enter a column number between 1 and %d \n",u->num_cols);
    return 0;
  }
  else if (abs(m.row)>u->num_rows){
    printf("The row you have tried to rotate is out of bounds, please enter a row number from -%d to %d and 0 if you don't want to rotate a row\n",u->num_rows,u->num_rows);
    return 0;
  }
  else if (*(u->mat + 0*u->num_cols + (m.column-1))!='.'){
    int j=0;
    printf("The column you have tried to place the token in is full \n");
    printf("These columns are available: \n");
    for(j=0;j<u->num_cols;j++){
      if (*(u->mat + 0*u->num_cols + (j))!='x' && *(u->mat + 0*u->num_cols + (j))!='o'){
        printf(" %d",j+1);
      }
    }
    printf("\n");
    return 0;
  }
  else{
    return 1;
  }
}

int hor_left(board,int,int,int,char); // implicit declaration of recursive function hor_left

int vert(board,int,int,int,char); // implicit declaration of recursive function vert

int diag_bot_right(board,int,int,int,char); // implicit declaration of recursive function diag_bot_right

int diag_bot_left(board,int,int,int,char); // implicit declaration of recursive function diag_bot_left

int diag_top_right(board,int,int,int,char); // implicit declaration of recursive function diag_top_right

int diag_top_left(board,int,int,int,char); // implicit declaration of recursive function diag_top_left

char is_winning_move(struct move m, board u){

  int i=0;int j=m.column-1; // loop variables

  if (m.row==0){ // if no row is rotated
    int l=4;int k=0;

    for (k=0;k<4;k++){ // checks horizontally at the 4 points adjacent to the new token for 4-in-a-rows 
      if (*(u->mat + (u->min_row)*u->num_cols + (j)) == u->token){
        if ((hor_left(u,u->min_row,j,0,u->token) == 1)){
          return u->token;
        }
      if (j==u->num_cols-1){
        j=0;
        }
      else {
      j++;}
      }
    }

    if (u->min_row<u->num_rows-3){
      if ((vert(u,u->min_row,m.column-1,0,u->token) == 1)){ // checks vertically at the new token for a 4 in a row
        return u->token;}
    }
    else {
      l=u->num_rows-u->min_row;
    }

    i=u->min_row;j=m.column-1;

    for (k=0;k<l;k++){
      if (*(u->mat + (i)*u->num_cols + (j)) == u->token){// checks diagonally-left at the 4 points adjacent to the new token for 4-in-a-rows 
        if ((diag_top_left(u,i,j,0,u->token) == 1)){
          return u->token;
        }
      if (j==u->num_cols-1){
        j=0;
      }
      else {
        j++;
      }
      i++;
      } 
    }

    i=u->min_row;j=m.column-1;
  
    for (k=0;k<l;k++){
      if (*(u->mat + (i)*u->num_cols + (j)) == u->token){// checks diagonally-right at the 4 points adjacent to the new token for 4-in-a-rows 
        if ((diag_top_right(u,i,j,0,u->token) == 1)){
          return u->token;
        }
      if (j==0){
        j=u->num_cols-1;
      }
      else {
        j--;
      }
      i++;
      } 
    }
    k=0;
    for (j=0;j<u->num_cols;j++){
      if (*(u->mat + (0)*u->num_cols + (j)) == 'x' ||*(u->mat + (0)*u->num_cols + (j)) =='o'){
        k++;
      }
    }
    if(k==u->num_cols){
      printf("Board is full \n");
      return 'd';
    }
    return '.';
  }
  else { // if a row is rotated more points will be checked for 4-in-a-rows
    char t='b';
    for (i=0;i<u->min_row+1;i++){ // traverses 0-min_row rows and checkes for 4-in-a-rows
      for (j=0;j<u->num_cols;j++){
        if (t!=*(u->mat + (i)*u->num_cols + (j)) && *(u->mat + (i)*u->num_cols + (j))!= '.'){
          if ((hor_left(u,i,j,0,*(u->mat + (i)*u->num_cols + (j))) == 1)){ // checking horizontally
            if(t=='b'){
              t=tolower(*(u->mat + (i)*u->num_cols + (j)));}
            else {
              return 'd';
            }
          }
          if (i<u->num_rows-3){
            if ((vert(u,i,j,0,*(u->mat + (i)*u->num_cols + (j))) == 1)){ // checking vertically
              if(t=='b'){
                t=tolower(*(u->mat + (i)*u->num_cols + (j)));}
              else {
                return 'd';
              }
            }
            if ((diag_bot_left(u,i,j,0,*(u->mat + (i)*u->num_cols + (j))) == 1)){ // checking diagonally-left
              if(t=='b'){
                t=tolower(*(u->mat + (i)*u->num_cols + (j)));}
              else {
                return 'd';
              }
            }
            if ((diag_bot_right(u,i,j,0,*(u->mat + (i)*u->num_cols + (j))) == 1)){ // checking diagonally-right
              if(t=='b'){
                t=tolower(*(u->mat + (i)*u->num_cols + (j)));}
              else {
                return 'd';
              }
            }
          }
        }
      }
    }

    if(t == 'x' || t=='o'){
      printf("Player %c has won the game",t);
      return t;
    }
    else{
      i=0;
      for (j=0;j<u->num_cols;j++){
        if (*(u->mat + (0)*u->num_cols + (j)) == 'x' ||*(u->mat + (0)*u->num_cols + (j)) =='o'){
          i++;
        }
      }
      if(i==u->num_cols){
        printf("Board is full \n");
        return 'd';
      }
      return '.';
    }
  }
}


void play_move(struct move m, board u){

  u->min_row=0;
  u->token=next_player(u);

  int i=0;int chk=1;int j=0; // variables used in loops

  while (chk){ // drops token at specified column 
    if (*(u->mat + (i+1)*u->num_cols + (m.column-1)) == '.'){
      i+=1;
    }
    else {
      *(u->mat + (i)*u->num_cols + (m.column-1)) = u->token;
      u->min_row=i;
      break;
    }
  }
  
  if (u->token == 'o'){ // updates token count
    u->num_o+=1;}
  else {
    u->num_x+=1;}
  
  int row = u->num_rows-abs(m.row);

  if(m.row==0){ // if no row is to be rotated
    u->current_winner=is_winning_move(m,u);
    if (u->current_winner=='d')
      printf("The Game is a Draw\n");
    else if (u->current_winner== 'x'){
      printf("Player x has won the game\n");
    }
    else if (u->current_winner== 'o'){
      printf("Player o has won the game\n");
    }
    return;
  }

  if (m.row>0){ // rotating to the right
    char temp1 =*(u->mat + (row)*u->num_cols + (u->num_cols-1));
    char temp2;
    for (j=0;j<u->num_cols;j++){
      temp2=*(u->mat + (row)*u->num_cols + (j));
      *(u->mat + (row)*u->num_cols + (j)) = temp1;
      temp1=temp2;
    }
  }
  else { //rotating to the left
    char temp1 =*(u->mat + (row)*u->num_cols + (0));
    char temp2;
    for (j=u->num_cols-1;j>-1;j--){
      temp2=*(u->mat + (row)*u->num_cols + (j));
      *(u->mat + (row)*u->num_cols + (j)) = temp1;
      temp1=temp2;
    }
  }

  char temp ; int k=0; // used in appying gravity

  for (i=row;i>-1;i--){ // gravity is applied to the board after rotation
    for (j=0;j<u->num_cols;j++){
      temp=*(u->mat + i*u->num_cols + j);
      k=i;chk=1;
      if (temp == 'x' || temp == 'o'){
        *(u->mat + i*u->num_cols + j)='.';
        while(chk){
          if (*(u->mat + (k+1)*u->num_cols + (j)) == '.'){
            k++;
          }
          else {
            if (u->min_row<k){
              u->min_row=k;
            }
            *(u->mat + (k)*u->num_cols + (j)) = temp;
            chk=0;
          }
        }
      }
    }
  }
  u->current_winner=is_winning_move(m,u);
  if (u->current_winner=='d')
      printf("The Game is a Draw\n");
}

int hor_left(board u,int i,int j,int chk,char c){ // recursive function for checking for and capitalizing 4-in-a-rows
  if (chk==4){
    if (j==u->num_cols-1)
      j=0;
    else 
      j+=1;
  }
  if (chk>3 && chk<8){
    *(u->mat + (i)*u->num_cols + (j))=toupper(*(u->mat + (i)*u->num_cols + (j)));
    if (j==u->num_cols-1)
      return hor_left(u,i,0,chk+1,c);
    return hor_left(u,i,j+1,chk+1,c);
  }
  if (chk>=8)
    return 1;
  if (*(u->mat + (i)*u->num_cols + (j))!=c){
    return 0;
  }
  else {
    if (j==0)
      return hor_left(u,i,u->num_cols-1,chk+1,c);
    return hor_left(u,i,j-1,chk+1,c);
  }
}

int vert(board u,int i,int j,int chk,char c){ // recursive function for checking for and capitalizing 4-in-a-rows
  if (chk==4){
    i-=1;
  }
  if (chk>3 && chk<8){
    *(u->mat + (i)*u->num_cols + (j))=toupper(*(u->mat + (i)*u->num_cols + (j)));
    return vert(u,i-1,j,chk+1,c);
  }
  if (chk>=8)
    return 1;
  if (*(u->mat + (i)*u->num_cols + (j))!=c){
    return 0;
  }
  else {
    return vert(u,i+1,j,chk+1,c);
  }
}

int diag_bot_right(board u,int i,int j,int chk,char c){ // recursive function for checking for and capitalizing 4-in-a-rows
  if (chk==4){
    if (j==0){
      j=u->num_cols-1;i-=1;}
    else{
      j-=1;i-=1;}
  }
  if (chk>3 && chk<8){
    *(u->mat + (i)*u->num_cols + (j))=toupper(*(u->mat + (i)*u->num_cols + (j)));
    if (j==0)
      return diag_bot_right(u,i-1,u->num_cols-1,chk+1,c);
    return diag_bot_right(u,i-1,j-1,chk+1,c);
  }
  if (chk>=8)
    return 1;
  if (*(u->mat + (i)*u->num_cols + (j))!=c){
    return 0;
  }
  else {
    if (j==u->num_cols-1){
      return diag_bot_right(u,i+1,0,chk+1,c);}
    return diag_bot_right(u,i+1,j+1,chk+1,c);
  }
}

int diag_bot_left(board u,int i,int j,int chk,char c){ // recursive function for checking for and capitalizing 4-in-a-rows
  if (chk==4){
    if (j==u->num_cols-1){
      j=0;i-=1;}
    else{
      j+=1;i-=1;}
  }

  if (chk>3 && chk<8){
    *(u->mat + (i)*u->num_cols + (j))=toupper(*(u->mat + (i)*u->num_cols + (j)));
    if (j==u->num_cols-1)
      return diag_bot_left(u,i-1,0,chk+1,c);
    return diag_bot_left(u,i-1,j+1,chk+1,c);
  }
  if (chk>=8)
    return 1;
  if (*(u->mat + (i)*u->num_cols + (j))!=c){
    return 0;
  }
  else {
    if (j==0){
      return diag_bot_left(u,i+1,u->num_cols-1,chk+1,c);}
    return diag_bot_left(u,i+1,j-1,chk+1,c);
  }
}

int diag_top_left(board u,int i,int j,int chk,char c){ // recursive function for checking for and capitalizing 4-in-a-rows
  if (chk==4){
    if (j==u->num_cols-1){
      j=0;i+=1;}
    else{
      j+=1;i+=1;}
  }

  if (chk>3 && chk<8){
    *(u->mat + (i)*u->num_cols + (j))=toupper(*(u->mat + (i)*u->num_cols + (j)));
    if (j==u->num_cols-1)
      return diag_top_left(u,i+1,0,chk+1,c);
    return diag_top_left(u,i+1,j+1,chk+1,c);
  }
  if (chk>=8)
    return 1;
  if (*(u->mat + (i)*u->num_cols + (j))!=c){
    return 0;
  }
  else {
    if (j==0){
      return diag_top_left(u,i-1,u->num_cols-1,chk+1,c);}
    return diag_top_left(u,i-1,j-1,chk+1,c);
  }
}

int diag_top_right(board u,int i,int j,int chk,char c){ // recursive function for checking for and capitalizing 4-in-a-rows
  if (chk==4){
    if (j==0){
      j=u->num_cols-1;i+=1;}
    else{
      j-=1;i+=1;}
  }

  if (chk>3 && chk<8){
    *(u->mat + (i)*u->num_cols + (j))=toupper(*(u->mat + (i)*u->num_cols + (j)));
    if (j==0)
      return diag_top_right(u,i+1,u->num_cols-1,chk+1,c);
    return diag_top_right(u,i+1,j-1,chk+1,c);
  }
  if (chk>=8)
    return 1;
  if (*(u->mat + (i)*u->num_cols + (j))!=c){
    return 0;
  }
  else {
    if (j==u->num_cols-1){
      return diag_top_right(u,i-1,0,chk+1,c);}
    return diag_top_right(u,i-1,j+1,chk+1,c);
  }
}

int clean_stdin() // cleans stdin
{
  while (getchar()!='\n');
  return 1;
}