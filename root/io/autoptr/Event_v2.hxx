#ifndef EVENT_V2_H
#define EVENT_V2_H

#include <RtypesCore.h>

#include <memory>

// Since std::auto_ptr is deprecated and may not exist anymore, we emulate it.
template <typename T>
struct EmulatedAutoPtr {
   T *fRawPtr = nullptr;

   EmulatedAutoPtr() = default;
   explicit EmulatedAutoPtr(T *rawPtr) : fRawPtr(rawPtr) {}
   EmulatedAutoPtr &operator=(EmulatedAutoPtr &other)
   {
      fRawPtr = other.fRawPtr;
      other.fRawPtr = nullptr;
      return *this;
   }
   EmulatedAutoPtr &operator=(T *rawPtr)
   {
      fRawPtr = rawPtr;
      return *this;
   }
};

struct Track {
   int fFoo;

   ClassDefNV(Track, 2)
};

struct Event {
   EmulatedAutoPtr<Track> fTrack;
   float fBar = 137.0;

   ClassDefNV(Event, 2)
};

#endif // EVENT_V2_H
