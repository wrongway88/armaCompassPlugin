while{true}
do
{
	_d = direction player;
	
	// hint format ["compass %1", _d];
	
	_result = "CompassClient" callExtension ["updateCompass", [_d]];
	
	// hint format ["result: %1 - %2 - %3", _result select 0, _result select 1, _result select 2];

	sleep 0.1;
};