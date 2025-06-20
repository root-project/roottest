#ifdef __ROOTCLING__

#pragma link C++ class Track+;
#pragma link C++ class Event+;
#pragma link C++ class Compat::DeprecatedAutoPtr<Track>+;

#pragma read sourceClass="EmulatedAutoPtr<Track>" targetClass="Compat::DeprecatedAutoPtr<Track>"

#pragma read sourceClass="Event" targetClass="Event" version="[2]" \
  source="Compat::DeprecatedAutoPtr<Track> fTrack" target="fTrack" \
  code="{ fTrack.release(); fTrack.reset(onfile.fTrack.fRawPtr); }"

#endif
