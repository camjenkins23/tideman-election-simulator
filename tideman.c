#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
char* candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, char* name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int indexof(int arr[], int value);
bool has_loop(int start, int loser);

int main(int argc, char* argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count;
    printf("Number of voters: ");
    scanf("%d", &voter_count);

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            char name[50];
            printf("Rank %d: ", j + 1);
            scanf("%49s", name);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, char* name, int ranks[])
{
    // TODO
    // if our vote matches a candidate, then we put said candidate in their respective rank
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // the ranks array is filled with the indexes of the candidates
    // in the order that the voter chose them in


    // this loop checks the candidates in our 2D array
    // if the index in rank is lower, which implies that said candidate is favored,
    // then we add a vote in the box that corresponds to our two candidates
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {

            // here we call our indexof function, which simply finds the index of a value in a given array
            if (indexof(ranks, i) < indexof(ranks, j))
            {
                preferences[i][j]++;
            }
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // in this case, i will loop through the rows of our 2d array and j will loop through the columns
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // preferences ij is the amount of votes that candidate i garnered
            // preferences ji is the amount of votes that candidate j garnered
            // if one is favored over the other, we arrange them in our pairs array accordingly
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                // we also keep track of how many pairs we are adding, so we can loop through our pairs later
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO

    // we sort through the counts larger than one as you cant sort something that does not have multiple elements
    if (pair_count > 1)
    {
        for (int j = 0; j < pair_count; j++)
        {
            for (int i = 0; i < (pair_count - 1); i++)
            {
                // we create a winner and loser variable for each of the compared items
                int i_winner = preferences[pairs[i].winner][pairs[i].loser];
                int i_loser = preferences[pairs[i].loser][pairs[i].winner];
                // we use our winner and loser variables to find the margin of votes between these two candidates
                int margin1 = i_winner - i_loser;

                //^^
                int next_winner = preferences[pairs[i + 1].winner][pairs[i + 1].loser];
                int next_loser = preferences[pairs[i + 1].loser][pairs[i + 1].winner];
                //^^
                int margin2 = next_winner - next_loser;

                // we only want to switch them if the margins are out of order, meaning the larger is on the right side
                // all of our larger margins should be on the left side
                if (margin2 > margin1)
                {
                    // we have to declare one of our values as a variable so we can refer to our original value after we change it
                    pair first = pairs[i];
                    pairs[i] = pairs[i + 1];
                    pairs[i + 1] = first;
                }
            }
        }
    }
    return;
}




// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        // we want to lock in our pair first, in order to determine if it creates a loop
        locked[pairs[i].winner][pairs[i].loser] = true;

        // if it creates a loop, we want to unlock this pair
        if (has_loop(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
}



// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // we look through all possible combination of winners and losers in our locked array
            // by iterating our first position first, we look down each column, instead of each row
            // our winner should not have any arrows pointing at them, which is what our first if looks for
            if (locked[j][i])
            {
                // if it is determined that this row has an arrow pointed at it, then we return and move to the next row
                break;
            }
            // if we get to the final spot in our row and we have determined that there are no arrows pointing at the candidate
            // then we have found our winner
            else if (j == (candidate_count - 1))
            {
                printf("The winner is: ");
                printf("%s\n", candidates[i]);
                return;
            }
        }
    }


    return;
}
// finds the index of the value in the given array
int indexof(int arr[], int value)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // here we search through each value in the array and determine if it is the value we are looking for
        if (arr[i] == value)
        {
            // since i increments through the values linearly we know that i will always represent the index of the value
            return i;
        }
    }
    return -1;
}

bool has_loop(int start, int loser)
{
    // if the end of the loop is our initial starting point, then it returns true
    // this implies that the given inputs do create a loop
    if (start == loser)
    {
        return true;
    }

    // if the loop is not ended, we look through our given loser to find if it is a winner elsewhere
    for (int i = 0; i < candidate_count; i++)
    {
        // if it is a winner, we call the same function that will find the winner for our next loser
        if (locked[loser][i])
        {
            // this calls our function recursively
            // if our function returns true, implying that our numbers do indeed cause a loop, we return true
            if (has_loop(start, i))
            {
                return true;
            }
        }
    }

    // finally, if our function finds that the loser does not win against another candidate
    // that would imply that the function does indeed end, thus we return false
    return false;

}