cimport media

cdef class Media:
    """Media class"""

    def __init__(self):
        pass

    def get_session_ids(self):
        return media.purple_media_get_session_ids(self)

    def send_dtmf(self, session_id, dtmf, volume, duration):
        """Send a DTMF signal out of band"""
        media.purple_media_send_dtmf((self, session_id, dtmf, volume, duration))
        return True
