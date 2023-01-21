class SailFaceException(Exception):
    """Base class for all exceptions raised by SailFace."""
    pass


class SDCardException(SailFaceException):
    """Raised when the SD card fails to mount."""
    pass

class PowerMonitorException(SailFaceException):
    """Raised when the power monitor encounters an error."""
    pass

class PowerMonitorBadReading(PowerMonitorException):
    """Raised when the power monitor encounters multiple bad reading."""
    pass
