void SpiderInit (void);
BOOL ProcessCommand (CDC &, char);
void DisplayColumn (CDC &, int);
void UpdateDeals ();
void DoColumnHints ();
int ColumnLength (int);

// The compiler is stupidly anal about this warning.

#pragma warning (disable:4244)
