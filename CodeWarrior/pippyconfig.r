/*
typedef struct SysAppPrefs {
Word priority; // task priority
DWord stackSize; // required stack space
DWord minHeapSpace; // minimum heap space required
} SysAppPrefsType;

priority = 30
stackSize = 0xD00
minHeapSpace = 0x1000

*/

type 'pref' {
	int priority;
	longint stacksize;
	longint minHeapSpace;
};

resource 'pref' (0, "Application Launch Prefs") {
	0x1E,
	0x2000,
	0x1000
};
