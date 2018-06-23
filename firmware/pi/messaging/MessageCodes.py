from enum import IntEnum, auto

class MessageCodes(IntEnum):
  LOG = 0
  ERROR = auto()
  START_TRACKING = auto()
  PAUSE_TRACKING = auto()
  
  TARGET_FOUND = auto()
  TARGET_LOST = auto()
  STOP_EVENT = auto()
  
  
  START_SYSTEM = 100
  STOP_SYSTEM = auto()
  SET_TRACKING_METHOD = auto()
  GET_FPS = auto()
  GET_TEMPERATURE = auto()
  
  _LAST = auto()