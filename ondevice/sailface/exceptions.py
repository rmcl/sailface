class SailFaceException(Exception):
    """Base class for all exceptions raised by SailFace."""
    pass


class SDCardException(SailFaceException):
    """Raised when the SD card fails to mount."""
    pass