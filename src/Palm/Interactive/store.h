#ifndef INC_STORE_H
#define INC_STORE_H

extern FieldPtr     inField;
extern FieldPtr     outField;
extern FormPtr      mainForm;
extern ScrollBarPtr scrollBar;

typedef struct {
	LocalID dbId;
	UInt16  recNr;  
} SourceRef;


#endif /* INC_STORE_H */

