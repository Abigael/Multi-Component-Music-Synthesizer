#pragma once
#include <string>

struct PianoNotes
{
	const TCHAR *name;
	std::string filename;
};

const PianoNotes pianonotes[] = { { TEXT("A0"), "A0" }, { TEXT("A0#"), "A0#" }, { TEXT("B0b"), "A0#" }, { TEXT("B0"), "B0" },
{TEXT("C1"), "C1"}, { TEXT("C1#"), "C1#" }, { TEXT("D1b"), "C1#" }, { TEXT("D1"), "D1" }, { TEXT("D1#"), "D1#" }, { TEXT("E1b"), "D1#" },
{TEXT("E1"), "E1"}, { TEXT("F1"), "F1" }, { TEXT("F1#"), "F1#" }, { TEXT("G1b"), "F1#" },
{TEXT("G1"), "G1"}, { TEXT("G1#"), "G1#" }, { TEXT("A1b"), "G1#" }, { TEXT("A1"), "A1" }, { TEXT("A1#"), "A1#" }, { TEXT("B1b"), "A1#" },
{TEXT("B1"), "B1"}, { TEXT("C2"), "C2" }, { TEXT("C2#"), "C2#" }, { TEXT("D2b"), "C2#" },
{TEXT("D2"), "D2"}, { TEXT("D2#"), "D2#" }, { TEXT("E2b"), "D2#" }, { TEXT("E2"), "E2" }, { TEXT("F2"), "F2" },
{TEXT("F2#"), "F2#"}, { TEXT("G2b"), "F2#" }, { TEXT("G2"), "G2" }, { TEXT("G2#"), "G2#" }, { TEXT("A2b"), "G2#" },
{TEXT("A2"), "A2"}, { TEXT("A2#"), "A2#" }, { TEXT("B2b"), "A2#" }, { TEXT("B2"), "B2" }, { TEXT("C3"), "C3" },
{TEXT("C3#"), "C3#"}, { TEXT("D3b"), "C3#" }, { TEXT("D3"), "D3" }, { TEXT("D3#"), "D3#" }, { TEXT("E3b"), "D3#" },
{TEXT("E3"), "E3"}, { TEXT("F3"), "F3" }, { TEXT("F3#"), "F3#" }, { TEXT("G3b"), "F3#" }, { TEXT("G3"), "G3" },
{TEXT("G3#"), "G3#"}, { TEXT("A3b"), "G3#" }, { TEXT("A3"), "A3" }, { TEXT("A3#"), "A3#" }, { TEXT("B3b"), "A3#" },
{TEXT("B3"), "B3"}, { TEXT("C4"), "C4" }, { TEXT("C4#"), "C4#" }, { TEXT("D4b"), "C4#" }, { TEXT("D4"), "D4" },
{TEXT("D4#"), "D4#"}, { TEXT("E4b"), "D4#" }, { TEXT("E4"), "E4" }, { TEXT("F4"), "F4" }, { TEXT("F4#"), "F4#" }, { TEXT("G4b"), "F4#" },
{TEXT("G4"), "G4"}, { TEXT("G4#"), "G4#" }, { TEXT("A4b"), "G4#" }, { TEXT("A4"), "A4" },
{TEXT("A4#"), "A4#"}, { TEXT("B4b"), "A4#" }, { TEXT("B4"), "B4" }, { TEXT("C5"), "C5" }, { TEXT("C5#"), "C5#" }, { TEXT("D5b"), "C5#" },
{TEXT("D5"), "D5"}, { TEXT("D5#"), "D5#" }, { TEXT("E5b"), "D5#" }, { TEXT("E5"), "E5" },
{TEXT("F5"), "F5"}, { TEXT("F5#"), "F5#" }, { TEXT("G5b"), "F5#" }, { TEXT("G5"), "G5" }, { TEXT("G5#"), "G5#" },
{TEXT("A5b"), "G5#"}, { TEXT("A5"), "A5" }, { TEXT("A5#"), "A5#" }, { TEXT("B5b"), "A5#" }, { TEXT("B5"), "B5" },
{TEXT("C6"), "C6"}, { TEXT("C6#"), "C6#" }, { TEXT("D6b"), "C6#" }, { TEXT("D6"), "D6" }, { TEXT("D6#"), "D6#" }, { TEXT("E6b"), "D6#" },
{TEXT("E6"), "E6"}, { TEXT("F6"), "F6" }, { TEXT("F6#"), "F6#" }, { TEXT("G6b"), "F6#" },
{TEXT("G6"), "G6"}, { TEXT("G6#"), "G6#" }, { TEXT("A6b"), "G6#" }, { TEXT("A6"), "A6" }, { TEXT("A6#"), "A6#" }, { TEXT("B6b"), "A6#" },
{TEXT("B6"), "B6"}, { TEXT("C7"), "C7" }, { TEXT("C7#"), "C7#" }, { TEXT("D7b"), "C7#" },
{TEXT("D7"), "D7"}, { TEXT("D7#"), "D7#" }, { TEXT("E7b"), "D7#" }, { TEXT("E7"), "E7" }, { TEXT("F7"), "F7" },
{TEXT("F7#"), "F7#"}, { TEXT("G7b"), "F7#" }, { TEXT("G7"), "G7" }, { TEXT("G7#"), "G7#" }, { TEXT("A7b"), "G7#" },
{TEXT("A7"), "A7"}, { TEXT("A7#"), "A7#" }, { TEXT("B7b"), "A7#" }, { TEXT("B7"), "B7" }, { TEXT("C8"), "C8" },
{TEXT("PedalU"), "pedalu"}, { TEXT("PedalD"), "pedald" }, { TEXT("A#0"), "A0#" }, { TEXT("Bb0"), "A0#" }, { TEXT("C#1"), "C1#" }, { TEXT("Db1"), "C1#" },
{TEXT("D#1"), "D1#"}, { TEXT("Eb1"), "D1#" }, { TEXT("F#1"), "F1#" }, { TEXT("Gb1"), "F1#" }, { TEXT("G#1"), "G1#" }, { TEXT("Ab1"), "G1#" },
{TEXT("A#1"), "A1#"}, { TEXT("Bb1"), "A1#" }, { TEXT("C#2"), "C2#" }, { TEXT("Db2"), "C2#" }, { TEXT("D#2"), "D2#" }, { TEXT("Eb2"), "D2#" },
{TEXT("F#2"), "F2#"}, { TEXT("Gb2"), "F2#" }, { TEXT("G#2"), "G2#" }, { TEXT("Ab2"), "G2#" }, { TEXT("A#2"), "A2#" }, { TEXT("Bb2"), "A2#" },
{TEXT("C#3"), "C3#"}, { TEXT("Db3"), "C3#" }, { TEXT("D#3"), "D3#" }, { TEXT("Eb3"), "D3#" }, { TEXT("F#3"), "F3#" }, { TEXT("Gb3"), "F3#" },
{TEXT("G#3"), "G3#"}, { TEXT("Ab3"), "G3#" }, { TEXT("A#3"), "A3#" }, { TEXT("Bb3"), "A3#" }, { TEXT("C#4"), "C4#" }, { TEXT("Db4"), "C4#" },
{TEXT("D#4"), "D4#"}, { TEXT("Eb4"), "D4#" }, { TEXT("F#4"), "F4#" }, { TEXT("Gb4"), "F4#" }, { TEXT("G#4"), "G4#" }, { TEXT("Ab4"), "G4#" },
{TEXT("A#4"), "A4#"}, { TEXT("Bb4"), "A4#" }, { TEXT("C#5"), "C5#" }, { TEXT("Db5"), "C5#" }, { TEXT("D#5"), "D5#" }, { TEXT("Eb5"), "D5#" },
{TEXT("F#5"), "F5#"}, { TEXT("Gb5"), "F5#" }, { TEXT("G#5"), "G5#" }, { TEXT("Ab5"), "G5#" }, { TEXT("A#5"), "A5#" }, { TEXT("Bb5"), "A5#" },
{TEXT("C#6"), "C6#"}, { TEXT("Db6"), "C6#" }, { TEXT("D#6"), "D6#" }, { TEXT("Eb6"), "D6#" }, { TEXT("F#6"), "F6#" }, { TEXT("Gb6"), "F6#" },
{TEXT("G#6"), "G6#"}, { TEXT("Ab6"), "G6#" }, { TEXT("A#6"), "A6#" }, { TEXT("Bb6"), "A6#" }, { TEXT("C#7"), "C7#" }, { TEXT("Db7"), "C7#" },
{TEXT("D#7"), "D7#"}, { TEXT("Eb7"), "D7#" }, { TEXT("F#7"), "F7#" }, { TEXT("Gb7"), "F7#" }, { TEXT("G#7"), "G7#" }, { TEXT("Ab7"), "G7#" },
{TEXT("A#7"), "A7#"}, { TEXT("Bb7"), "A7#" }, };


const char* NoteToFile(const WCHAR *name, char);

