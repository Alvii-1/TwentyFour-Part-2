// 
// Zohaib Alvi
// Program 3: TwentyFour Part 2
//
/* Program 3 is a continuation of Program 1, the original TwentyFour game which used arrays as the primary
data structure. In Program 3, however, a stack is used in the operations of PEMDAS to perform the calculations.
In this implementation, there is also an easy, medium, and hard mode, and the user must choose a single difficulty
from which the game will choose a random puzzle. The stack uses a linked list implementation, one stack deals with 
only chars from the user inputted expression, the other deals with ints and actually evaluating the expression.  
*/
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// VALGRIND commands used to detect leaks:
// ----------------------------------------
// gcc -o test -g prog3-starter-code.c     (ran on initial compile) 
// valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./test

// Below this point, the struct and its functions which deal with taking the user input from the inputted
// expression. This stack is primarily used to convert the expression from infix to postfix notation.
// This struct is known as the "Char handling struct"
struct node {
  char data; // this stack uses only char data types
  struct node* prev; // Using a prev (previous) pointer, since a stack has a top and only previous items below
};

/* Below, the constructor which makes nodes for this struct. It intakes a char data value to set, and first makes
a new node object of the struct. Then it allocates memory, assigns the data value, and the previous pointer. Returns 
the newly created and completed node. */
struct node* makeNewNode(char data) {
  struct node* node;
  node = (struct node*) malloc(sizeof(struct node));
  node->data = data;
  node->prev = NULL;
  return node;
}

/* The push function for the char handling struct. It intakes a reference to the head of the linked list, and a char
data type. This is the function called whenever a new object is pushed. It then sets the previous pointer of the newly
added node to point to the hold head_ref or reference to the head pointer. It then reassigns the head as the new top. */
void push(struct node **head_ref, char data) {
  struct node* node = makeNewNode(data);
  node->prev = (*head_ref);
  (*head_ref) = node;
}

/* The pop function for the char handling struct. It intakes a reference to the head of the list, and creates a new 
temporary node which is assigned the head. The head is then moved backwards to the object below on the stack, and the
data value of the node pointed to by temp is assigned to pop, the normal variable to return that data. Temp node is freed */
char pop(struct node** head_ref) {
  struct node* temp = *head_ref;
  (*head_ref) = (*head_ref)->prev;
  char pop = temp->data;
  free(temp);
  return pop;
}

/* The isEmpty function for the char handling struct. It intakes a reference to the head (top) of the struct, and returns
1 if the head is empty (the stack is empty in that case). Otherwise, it returns 0 indicating the stack is not empty. */
int isEmpty(struct node** head_ref) {
  if (*head_ref == NULL) {
    return 1;
  }
  return 0;
}

// === Below this point are the functions and struct for the stack which handles evaluating the expression === //

/* The struct intNode is also known as the int handling struct. It handles creating the stack for the evaluation 
of the postfix expression. It uses an integer value (val) as the data, and like before a pointer to the previous node */ 
struct intNode {
  int val;
  struct intNode* prev;
};

/* The makeNewIntNode function is the same as the previous makeNewNode function for the char handling struct, except 
this time it assigns the integer val as the data of the node. It uses malloc to create space in the memory for the
node, assigns its integer val, sets its previous to NULL and returns the node. */
struct intNode* makeNewIntNode(int val) {
  struct intNode* node;
  node = (struct intNode*) malloc(sizeof(struct intNode));
  node->val = val;
  node->prev = NULL;
  return node;
}

/* The intPush function for the int handling struct works the same as the push function of the char handling struct
except it uses an integer value to assign as the data. It still takes a reference to the head (top) of the stack,
reassigning the new node to be the new head and setting its previous to point to any old node or NULL */
void intPush(struct intNode** head_ref, int val) {
  struct intNode* node = makeNewIntNode(val);
  node->prev = (*head_ref);
  (*head_ref) = node;
}

/* intPop works similar to the pop function of the previous char handling struct. This time, it uses an integer
val to assign as the data type, and returns an integer instead of a char. A temp node is set to the old head (top) 
of the stack, the head (top) is moved down, the data value is taken from the old head (temp) and temp is freed, and 
the data value integer is returned from the function */
int intPop(struct intNode** head_ref) {
  struct intNode* temp = *head_ref;
  (*head_ref) = (*head_ref)->prev;
  int pop = temp->val;
  free(temp);
  return pop;
}

/* The iIntStackEmpty works the same as the isEmpty function, taking a reference to the head (top) of the stack
as a parameter. If the head is NULL, the stack is empty, return 1. otherwise the stack is not empty, return 0. */
int isIntStackEmpty(struct intNode** head_ref) {
  if (*head_ref == NULL) {
    return 1;
  }
  return 0;
}

// === Below this point are the normal functions which handle certain jobs within the program == //

/* The introMessage function takes no parameters and outputs the intro message 
that is only shown once per each time that the user plays the twenty-four game. */
void introMessage() {
  printf("Welcome to the game of TwentyFour Part Two!\n");
	printf("Use each of the four numbers shown exactly once, \n");
	printf("combining them somehow with the basic mathematical operators (+,-,*,/) \n");
	printf("to yield the value twenty-four.\n");
}

/* The getDiffLvl function takes no parameters. It first outputs the prompt for the user to choose
the difficulty level of the program. It then takes the input, and depending on the character input
of the user, it assigns the integer 0 for easy, 1 for medium, and 2 for hard. It returns this integer */
int getDiffLvl() {
  char diffLvl;
  
  printf("Choose your difficulty level: E for easy, M for medium, and H for hard (default is easy).\n");
  printf("Your choice --> ");
  scanf("%1s", &diffLvl);
  while (getchar() != '\n') {
  }
  if (diffLvl == 'H') {
    return 2;
  }
  else if (diffLvl == 'M') {
    return 1;
  }  
  
  return 0;
}

/* The fillPuzzleArray function takes in the filename which was determined in main(), and an empty array 
which will be dynamically resized and filled. It opens the file and dynamically fills the puzzleArray in a 
while loop, using realloc and malloc to create space alongside the numPuzzles counter to track the number of 
puzzles being put into the array. The file is opened at the beginning and closed at the end */
void fillPuzzleArray(char *playPuzzle, char*** puzzleArray, int *numPuzzles) {
  char fileBuffer[100]; // initializing a buffer to hold the file contents immediately after they're read
  *numPuzzles = 0;
  FILE *file  = fopen(playPuzzle, "r"); // opening the file for reading, similar to program 2

  /* The below while loop uses fgets() to get the entire line string from the file. It then
  dynamically resizes the array of puzzles as they're read from the file and stores each line
  in the file as its own element. Ex. the line "4 4 7 8" is stored at index 0 after space is allocated */
  while (fgets(fileBuffer, sizeof(fileBuffer), file)) {
    *puzzleArray = realloc(*puzzleArray, ((*numPuzzles) + 1) * sizeof(char*));
    (*puzzleArray)[*numPuzzles] = malloc(strlen(fileBuffer) + 1);
    strcpy((*puzzleArray)[*numPuzzles], fileBuffer);
    (*numPuzzles)++;
  }

  fclose(file);
}

/* The checkForOperand function takes a number as a character as a parameter. It has a fixed sized array of
numbers as characters, and if the parameter matches any one of those in the for loop, 1 is returned. If the
for loop completes without finding a match, 0 is returned, indicating the char is not a number. */
int checkForOperand(char num) {
  char charNums[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  for (int i = 0; i < 9; i++) {
    if (num == charNums[i]) {
      return 1;
    }
  }
    
  return 0;
}

/* The checkForOperator function takes in a char operation as a parameter. It checks if that char is 
the same as any one of the valid operations in math for this program. If it is, 1 is returned. Otherwise,
0 is returned, indicating the char is not a mathematical operation. */
int checkForOperator(char operation) {
  if (operation == '*' || operation == '/' || operation == '+' || operation == '-') {
    return 1;
  }
  return 0;
}

/* The checkPEMDAS function takes a reference to the head (top) of a stack and a character as a paremeter.
If the data on the head of the stack is a * or /, the precedence is set to 1, lower precedence meaning it
should be evaluated first. Otherwise, if the character is a + or -, its precedence is 2. The same is done
for the char operation parameter. At the end, if the operation parameter has a higher precedence, we can return 1.
If this is not the case, return 0. */
int checkPEMDAS(struct node** head_ref, char operation) {
  int firstPrecedence = 0, secPrecedence = 0;
  
  if ((*head_ref)->data == '*' || ((*head_ref)->data == '/')) {
    firstPrecedence = 1;
  }
  else if ((*head_ref)->data == '+' || ((*head_ref)->data == '-')) {
    firstPrecedence = 2;
  }
  
  if (operation == '*' || operation == '/') {
    secPrecedence = 1;
  }
  else if (operation == '+' || operation == '-') {
    secPrecedence = 2;
  }

  if (firstPrecedence <= secPrecedence) {
    return 1;
  }
  
  return 0;
}

/* The checkOpenParentheses function takes in a reference to the head of the stack (the top) as a parameter.
It checks if the data value of the head is an open parentheses. If it is, return 1, otherwise return 0. */
int checkOpenParentheses(struct node**head_ref) {
  if ((*head_ref)->data == '(') {
    return 1;
  }
  return 0;
}

/* The calculate function takes in the two operands and the operation to be done between them, carrying
out the operation based on what the operation matches with in the if and else if statements. The total
is calculated with that operation and it is then returned.*/
int calculate(int num1, int num2, char operation) {
  int total;

  if (operation == '*') {
    total = num1 * num2;
  }
  else if (operation == '/') {
    total = num1 / num2;
  }
  else if (operation == '+') {
    total = num1 + num2;
  }
  else if (operation == '-') {
    total = num1 - num2;
  }
  
  return total;
}

/* The doMath function takes in the string (char array) which holds the postfix expression. First, it
"cleans" the postfix string and sets the cleaned string to be evalStr. It then creates a new stack of 
integers, looping through the new evalStr to push operands (ints) onto the stack, and evaluate them with
the operators. At the end, anything still on the stack is popped and an error message is displayed if the stack
is not cleared by the end of the for loop, after the intPop function after the for loop. This call to intPop removes
the final total value from the stack. */
int doMath(char* postfixStr) {
  char operation;
  char evalStr[100] = {'\0'};
  int singleDigit = 0;
  int evalTotal[100] = {0};
  int total, j = 0, num1, num2, operand;

  // Below, looping through the postfixStr to "clean" it. It may have erroneous chars in it, so we only need the operators
  // and the operands to add into the evalStr, the string which holds everything used in the evaluations of expressions. 
  for (int i = 0; postfixStr[i] != '\0'; i++) {
    if (checkForOperand(postfixStr[i]) || checkForOperator(postfixStr[i])) {
      evalStr[j] = postfixStr[i];
      j += 1;
    }
  }

  struct intNode* evalStack = NULL; // initializing a new integer stack, it will only ever hold operands and the final total
  
  for (int i = 0; evalStr[i] != '\0'; i++) { // looping through the evalStr array, which holds operands and operators
    if (checkForOperand(evalStr[i])) {  // if the item looked at is an operand
      operand = evalStr[i] - '0'; // converting the operand value from its char type to its int type using ASCII conversion
      intPush(&evalStack, operand); // pushing it onto the stack
    }
    else if (checkForOperator(evalStr[i])) { // if the item looked at is an operator, we can evaluate last 2 operands
      num2 = intPop(&evalStack); // here and below, getting the 2 operands off the stack
      num1 = intPop(&evalStack);
      operation = evalStr[i]; // isolating the operation to be done, * / + -
      total = calculate(num1, num2, operation); // calculating the total
      printf("%d %c %d = %d.\n", num1, operation, num2, total);
      intPush(&evalStack, total);
    }
  }
  intPop(&evalStack); // popping the final total off the stack. Below, if the stack still isnt empty, theres too many values
  if (!isIntStackEmpty(&evalStack)) {
    printf("Error! Too many values in the expression.\n");
    while (!isIntStackEmpty(&evalStack)) {
      intPop(&evalStack); // popping everything remaining off the stack
    }
    return -1; // returning -1 if the error is encountered. Otherwise, return the total below 
  }
  
  return total;
}

/* The convertInfixPostfix expression converts the user's expression from infix (regular) to postfix form. 1 * 1 becomes
11*. It creates a character stack to handle the conversion, and also handles error checking for parentheses. If there
are more closing than opening parenthesis, there is an error. If the for loop encounters a lower priority PEMDAS operator
at any point, the ones currently on the stack are pushed onto the postfixStr array. We then call doMath so that function
can use the integer array to evaluate the string in postfix notation */
int convertInfixPostfix(char* infixStr) {
  struct node* stack = NULL; 
  int j = 0, totalFromMath;
  char postfixStr[100] = {'\0'}; 
  int startParentheses = 0, closeParentheses = 0;

  for (int i = 0; infixStr[i] != '\0'; i++) { // looping through the user's infix string which they entered
    if (checkForOperand(infixStr[i])) {  // add all operands to the result string like normal
      postfixStr[j] = infixStr[i];
      j += 1;
    }
    else if (infixStr[i] == '(') {  // all opening parenthesis are added to the STACK
      push(&stack, infixStr[i]);
      startParentheses += 1; // counting parentheses for error checking
    }
    else if (infixStr[i] == ')') {
      // if there is a closing parentheses, everything must be evaluated and the opening parentheses must pop off stack
      while (!isEmpty(&stack) && !checkOpenParentheses(&stack)) {
        postfixStr[j] = pop(&stack);
        j += 1;
      }
      if (!isEmpty(&stack)) {
        pop(&stack);
      }
      closeParentheses += 1;

      if (closeParentheses > startParentheses) {
        break; // here, if there is more closing than opening parenthesis, there is problem so stop evaluating
      }
      
    }
    else if (checkForOperator(infixStr[i])) {
      // add the first operator to the stack. Then add the second , then check for precedence with the next. 
      // If the next is higher, pop the previous 
      // if we're looking at an operator with a lower precedence from the stack head, pop old stack head into result and push current operator
      while (!isEmpty(&stack) && checkPEMDAS(&stack, infixStr[i]) && !checkOpenParentheses(&stack)) { 
        postfixStr[j] =  pop(&stack);
        j += 1;
      }
      push(&stack, infixStr[i]); // push the next item onto the stack
    }
  }
  
  while (!isEmpty(&stack)) {
    postfixStr[j] = pop(&stack); // if anything is left in the stack, pop those items into the postfixStr
    j += 1;
  }

  totalFromMath = doMath(postfixStr);  // call a function to do the math and get the total of the expression
  
  if (startParentheses < closeParentheses) { // here and below if statement, checking errors with parenthesis and returning -1 indicates error
    printf("Error! Too many closing parentheses in the expression.");
      return -1;
  }
  if (startParentheses > closeParentheses) {
    printf("Error! Too many opening parentheses in the expression.");
      return -1;
  }
  return totalFromMath; // otherwise just return the total of the expression
}

/* The checkAllNumsUsed function is used to ensure all the numbers in the user inputted expression exist in the generated
puzzle from the game. The parameters are the user's expression answerBuffer, and the numDisplay which holds the puzzle nums.*/
int checkAllNumsUsed(char* answerBuffer, char* numDisplay) {
  int used[4] = {0, 0, 0, 0}; // each index corresponds to a single operand from left to right

  /* Below for loop checks to ensure a number is first a operand, and then if it is one of the 4 numbers in
  the generated puzzle. If it is, we can go through and find out which one it is and reassign that number to -1
  so we avoid recounting the same user inputted number for the same puzzle num. The used[] array is updated accordingly*/
  for (int i = 0; answerBuffer[i] != '\0'; i++) {
    if (checkForOperand(answerBuffer[i])) {
      if (answerBuffer[i] == numDisplay[0] || answerBuffer[i] == numDisplay[2] || 
          answerBuffer[i] == numDisplay[4] || answerBuffer[i] == numDisplay[6]) {
        
        if (answerBuffer[i] == numDisplay[0]) {
          numDisplay[0] = -1;
          used[0] = 1;
        }
        else if (answerBuffer[i] == numDisplay[2]) {
          numDisplay[2] = -1;
          used[1] = 1;
        }
        else if (answerBuffer[i] == numDisplay[4]) {
          numDisplay[4] = -1;
          used[2] = 1;
        }
        else if (answerBuffer[i] == numDisplay[6]) {
          numDisplay[6] = -1;
          used[3] = 1;
        }
      }
      else {
        return 0; // if a match is never found, we return 0
      }
    }
  }

  for (int j = 0; j < 4; j++) {
    if (used[j] == 0) {
      return 0; // if a match is never found and the for loop doesn't catch it, return 0
    }
  }

  return 1; // if a match is found, return 1
}

/* The checkAllValidOps function takes in the user inputted expression in the answerBuffer string array. It will loop through
the expression the user has given, and if the answer contains none of the Operands, Operators, Parenthesis, Spaces, or nul chars,
it will count that character which doesnt match to those as an invalid operator. */
int checkAllValidOps(char* answerBuffer) {
  int invalidOpCounter = 0, operatorCounter = 0;
  
  for (int i = 0; answerBuffer[i] != '\0'; i++) {
    if (checkForOperand(answerBuffer[i])) {
      continue;
    }
    else if (checkForOperator(answerBuffer[i])) {
      operatorCounter += 1;
      continue;
    }
    else if (answerBuffer[i] == ')' || answerBuffer[i] == '(' || answerBuffer[i] == ' ' || answerBuffer[i] == '\n') {
      continue;
    }
    invalidOpCounter += 1; // if the for loop makes it here, it hasn't hit a continue and therefore is an invalid operator
  }

  if (operatorCounter == 3) {
    return 0; // if we have exactly 3 operators which are valid, everything is fine so we return 0
  }

  if (invalidOpCounter > 0) { // If there is a single invalidOperator or more, there is an invalid symbol, return 1
    printf("Error! Invalid symbol entered. Please try again.\n\n");
    return 1;
  }
  
  return 0; // otherwise, if we make it here everything is valid, so return 0
}


int main() {
  srand(1);
	introMessage(); // display intro msg
  int diffLvl = 0, numPuzzles = 0, playAgain = 0;
  char** puzzleArray = NULL; // the array which holds each puzzle from the file

  /* The below while loop is what handles the user playing the game multiple times and changing difficulties.
  The while loop is only exited when the user enters 3 for the option "playAgain" */
  while (playAgain != 3) { 
    int gameCode = 0, errorCode;
    char answerBuffer[100];
    char numDisplay[9];  /* a fixed sized area holds the numbers to display below. The array is fixed since each 
                        line has only 8 characters; 4 numbers, 3 spaces, end of string char, and newline char. */
    if (playAgain != 1) {
      if (playAgain == 2 || playAgain == 3) { // any option 2 or 3 on replay frees previous play's memory
        for (int i = 0; i < numPuzzles; i++) {
          if (puzzleArray[i] != NULL) {
            free(puzzleArray[i]); // freeing the puzzleArray before reassigning it with another difficulty puzzle
          }
        }
        
        *puzzleArray = NULL;
      }
    
      numPuzzles = 0; // resetting the number of puzzles since this is a new difficulty / run of the game
      diffLvl = getDiffLvl(); // get the difficulty as a char, return a corresponding int

      if (diffLvl == 2) { // here and below, the return from diffLvl tells us which file to load and dynamically fill
        char playPuzzle[] = "hard.txt";
        fillPuzzleArray(playPuzzle, &puzzleArray, &numPuzzles);
      }
      else if (diffLvl == 1) {
        char playPuzzle[] = "medium.txt";
        fillPuzzleArray(playPuzzle, &puzzleArray, &numPuzzles);
      }
      else {
        char playPuzzle[] = "easy.txt";
        fillPuzzleArray(playPuzzle, &puzzleArray, &numPuzzles);
      }
    }

    strcpy(numDisplay, puzzleArray[rand() % numPuzzles]); // the randomly generated puzzle is copied and output below
    printf("The numbers to use are: %c, %c, %c, %c.\n", numDisplay[0], numDisplay[2], numDisplay[4], numDisplay[6]);
    printf("Enter your solution: ");
    fgets(answerBuffer, sizeof(answerBuffer), stdin); // fgets gets the user's expression

    // Below, these two if statements check first the valid operators and if all numbers are used for error checking
    if (checkAllValidOps(answerBuffer)) {
      playAgain = 1; // playAgain = 1, so we play the same puzzle easy/med/hard again.
      continue;
    }
    if (checkAllNumsUsed(answerBuffer, numDisplay) == 0) {
      printf("Error! You must use all four numbers, and use each one only once. Please try again.\n\n");
      playAgain = 1;
      continue;
    }
    
    gameCode = convertInfixPostfix(answerBuffer);

    // Below, if the gameCode returns 24, the game is won. Otherwise, if the game code is less than -1 or not 24, the game is lost
    if (gameCode == 24) {
      printf("Well done! You are a math genius.\n");
    }
    else if (gameCode > -1 && gameCode != 24) {
      printf("Sorry. Your solution did not evaluate to 24.\n");
    }
    
    printf("\nEnter: \t1 to play again,\n"); // here and below, prompting the user to play again
    printf("\t2 to change the difficulty level and then play again, or\n");
    printf("\t3 to exit the program.\n");
    printf("Your choice --> ");
    scanf("%d", &playAgain);
    getchar(); // removing the final newline char so we don't get buffer overflow
  }

  if (playAgain == 2 || playAgain == 3) { // A final check which ensures play memory is freed when 3 is input
      for (int i = 0; i < numPuzzles; i++) {
        if (puzzleArray[i] != NULL) {
          free(puzzleArray[i]);
        }
      }
    
      *puzzleArray = NULL;
  }
  
  free(puzzleArray); // finally freeing the non fixed sized array holding the puzzles 
  
	printf("\nThanks for playing!\n");
  printf("Exiting...\n");
	return 0;
}
