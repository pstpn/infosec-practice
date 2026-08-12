# Information security labs

Four labs, each implementing a cipher or a compression scheme from scratch in C++ and
running it over the same three kinds of input, a text file, a BMP image and a zip archive.
Using the same inputs everywhere is deliberate, since an image makes it obvious when a mode
leaks structure and an archive shows what happens to data that is already incompressible.

| Lab | Topic |
|-----|-------|
| [lab_01](lab_01) | The Enigma machine, rotors, reflector and plugboard |
| [lab_02](lab_02) | DES in CFB mode |
| [lab_03](lab_03) | AES in PCBC mode |
| [lab_05](lab_05) | A file archiver built on Huffman coding |

Each lab keeps its sources at the root or in `src/`, the input files in `data/` and a LaTeX
report with the analysis in `doc/` or `report/`.
