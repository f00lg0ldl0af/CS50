#include <cs50.h>
#include <stdio.h>
#include <string.h>

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
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

// Global variables
int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
bool creates_cycle(int winner, int loser);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
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
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();  // For each pair of candidate, preferred candidate and margin by which they are preferred
    sort_pairs(); // Sort pairs in decreasing order of preferred margin
    lock_pairs(); // Start with strongest pair, lock in each pair to graph and not create a cycle in graph
    print_winner();
    return 0;
}

// Update ranks given a new vote like 0, Kellie, int ranks[]
bool vote(int rank, string name, int ranks[]) // j, name, ranks
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i; /* Update array to indicate voter
            has candidate (index) as rank preference */
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // Iterate through each candidate as the preferred candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Iterate through less preferred candidate from i-th candidate
        for (int j = i + 1; j < candidate_count; j++)
        {
            // preferences[a][b] is no. of voters prefer a over b
            // Retrieve the indexes of preferred candidate and less preferred candidate
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // Iterate over preferences array, starting with preferred candidate i
    for (int i = 0; i < candidate_count; i++)
    {
        // Iterate over less preferred candidate
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Check if candidate i preferred over candidate j
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            // Check if candidate j preferred over candidate i
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
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
    // Bubble sort where greatest pair bubbles up to be the last pair of pair array
    // Exclude last pair since no need to compare subsequent pair
    for (int i = 0; i < pair_count - 1; i++)
    {
        // Iterate through unsorted portion of pairs array
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            // Retrieve no. of votes from candidates by accessing preferences
            int votes1 = preferences[pairs[j].winner][pairs[j].loser];
            int votes2 = preferences[pairs[j + 1].winner][pairs[j + 1].loser];

            if (votes1 < votes2)
            {
                // Swap positions of current pair
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // Iterate over the pairs
    for (int i = 0; i < pair_count; i++)
    {
        // Lock in a given pair given there is no cycle
        if (!creates_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Check if adding edge creates a cycle
// locked[i][j] means i is locked in over j
// bool locked[MAX][MAX];
bool creates_cycle(int winner, int loser) // Winner's index, loser's index
{
    // Base Case
    if (winner == loser)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i]) // Check if locked edge between loser and candidate i
        {
            if (creates_cycle(winner, i))
            {
                return true;
            }
        }
    }
    return false;

    // ALTERNATIVE
    // How do we know if there is no cycle
    // Find way to break loop when current winner has no else locked on itself

    // Iterate over candidates to check if any lock into current winner of particular pair
    /*while (winner != -1 && winner != loser)
    {
        bool source_found = false;

        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][winner])
            {
                source_found = true;
                winner = i; // Mark current candidate as winner (temporarily)
            }
        }

        if (source_found == false) // If no one locked into current winner
        {
            winner = -1;
        }
    }

    if (winner == loser)
    {
        return true;
    }
    return false;*/

    // return 0;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // Iterate through locked array
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_source = true;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                is_source = false;
                break;
            }
        }

        if (is_source)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }

    return;
}