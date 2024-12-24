# Tideman Voting System

## Overview
The Tideman voting system is a ranked-choice voting algorithm that determines the winner by comparing the preferences of the voters by building a directed graph of pairs. This script implements the Tideman voting system in C and handles the necessary steps from collecting voter preferences to determining the election winner.

## Features
* Handles up to 9 candidates. 
* Ensures the voters rank all the candidates.
* Validates inputs and allows for correction of invalid votes.
* Uses a sorting algorithm to order candidate pairs by margin of victory.
* Prevents cycles in the graph when locking in pairs.
* Determines the winner based on the graph of locked pairs.

## Files
* tideman.c: The main script implementing the algorithm.

## Usage
### 1. Compile the Program
Use gcc to compile the script:
```bash
gcc tideman.c -o tideman
```
### 2. Run the Program
Provide the candidate names as command-line arguments:
```bash
./tideman Alice Bob Charlie
```
### 3. Input Voter Data
* Enter the number of voters when prompted.
* For each voter, rank the candidates in order of preference by typing their names.
* If an invalid name is entered, the program will prompt for a valid input.

### 4. Output
The program will output the name of the winning candidate.
#### Example  
##### Input  
```bash
./tideman Alice Bob Charlie
Number of voters: 3
Rank 1: Alice
Rank 2: Bob
Rank 3: Charlie

Rank 1: Charlie
Rank 2: Alice
Rank 3: Bob

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice
```
##### Output 
```bash
The winner is: Alice
```

## Constraints
* Maximum of 9 candidates.
* Candidate names must be unique.
* Voters must rank all candidates without skipping ranks.

## Error Handling
* Invalid candidate names are rejected, and the user is prompted to try again.
* Exits if the number of candidates exceeds the limit.

## Future Improvements
* Support for more than 9 candidates.
* Enhanced user interface for better usability.
* Integration with a database for storing votes.