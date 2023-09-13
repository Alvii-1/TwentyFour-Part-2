# TwentyFour-Part-2
TwentyFour is a math game in which the player is presented 4 numbers and they must determine a way to combine those numbers using mathematical operations to get 24 as the result. Each number must be used exactly once. The user may play a variety of puzzles and difficulties.

Part 2 is a continuation of Part 1, the original TwentyFour game which used arrays as the primary
data structure. In Part 2, however, a stack is used in the operations of PEMDAS to perform the calculations.
In this implementation, there is also an easy, medium, and hard mode, and the user must choose a single difficulty
from which the game will choose a random puzzle. The stack uses a linked list implementation, one stack deals with 
only chars from the user inputted expression, the other deals with ints and actually evaluating the expression.
