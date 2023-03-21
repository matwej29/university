def CountNucleotides(dna):
    nucleotide_frequencies = {'A': 0, 'C': 0, 'G': 0, 'T': 0}

    for nucleotide in dna:
        nucleotide_frequencies[nucleotide] += 1

    return nucleotide_frequencies.values()


# print(*CountNucleotides(input()))
print(*CountNucleotides('AGCTTTTCATTCTGACTGCAACGGGCAATATGTCTCTGTGTGGATTAAAAAAAGAGTGTCTGATAGCAGC'))
