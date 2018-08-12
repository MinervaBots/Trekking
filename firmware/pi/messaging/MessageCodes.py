from enum import IntEnum

class MessageCodes(IntEnum):
    LOG = 0
    ERROR = 1
    START_TRACKING = 2
    PAUSE_TRACKING = 3

    TARGET_FOUND = 4
    TARGET_LOST = 5
    STOP_EVENT = 6
    TEMPERATURE = 7

    START_SYSTEM = 100
    STOP_SYSTEM = 101
    SET_TRACKING_METHOD = 102
    GET_FPS = 103
    GET_TEMPERATURE = 104