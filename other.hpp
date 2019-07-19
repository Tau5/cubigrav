int Adjust( int OldValue , int OldMin , int OldMax, int NewMin, int NewMax)
{
    int OldRange = (OldMax - OldMin);  
	int NewRange = (NewMax - NewMin);  
	int NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin;
	return NewValue;
}