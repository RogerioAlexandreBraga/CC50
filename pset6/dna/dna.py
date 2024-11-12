import csv
from sys import argv, exit


def main():
    if len(argv) != 3:
        print(f"Usage: python {argv[0]} data.csv sequence.txt")
        exit(1)

    _, data_file, sequence_file = argv

    content = read_csv(data_file)
    seq_count = read_dna_sequence(sequence_file, content["columns"][1:])

    print(identify_dna(content["data"], seq_count))
    exit(0)


def read_csv(path):
    with open(path) as csvfile:
        reader = csv.DictReader(csvfile)
        return {"columns": reader.fieldnames, "data": list(reader)}


def max_consecutive_reps(substring, string):
    total_reps = len(string)

    while (substring * total_reps) not in string and total_reps > 0:
        total_reps -= 1

    return total_reps


def read_dna_sequence(path, sequences):
    with open(path) as file:
        dna_seq = file.read()

        return {seq: str(max_consecutive_reps(seq, dna_seq)) for seq in sequences}


def identify_dna(user_data, seq_count):
    return next(
        (data["name"] for data in user_data if seq_count.items() <= data.items()),
        "No match",
    )


main()

