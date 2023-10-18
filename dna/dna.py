import csv
import sys


def main():
    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    database = []
    filename = sys.argv[1]
    with open(filename) as f:
        reader = csv.DictReader(f)
        for row in reader:
            database.append(row)

    # TODO: Read DNA sequence file into a variable
    filename = sys.argv[2]
    with open(filename) as f:
        sequence = f.read()

    # TODO: Find longest match of each STR in DNA sequence
    subsequences = list(database[0].keys())[1:]
    result = {}  # Save STR counts in a dict
    for subsequence in subsequences:
        result[subsequence] = longest_match(sequence, subsequence)

    # TODO: Check database for matching profiles
    for (
        profile
    ) in database:  # For each row in subsequence[], check if STR count matches
        match = 0
        for subsequence in subsequences:
            if int(profile[subsequence]) == int(result[subsequence]):
                match += 1
        if match == len(subsequences):
            print(profile["name"])
            return

    print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break  # Exits WHILE loop, update longest_run

        # Update most consecutive matches found
        longest_run = max(longest_run, count)  # After this line, jumps to next i

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
