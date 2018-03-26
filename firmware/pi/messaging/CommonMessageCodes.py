from enum import IntEnum, auto

class CommonMessageCodes(IntEnum):
	
	ERROR = 1
	START_TRACKING = auto()
	PAUSE_TRACKING = auto()
	
	_LAST = auto()