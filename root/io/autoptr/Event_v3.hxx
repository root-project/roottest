#ifndef EVENT_V3_H
#define EVENT_V3_H

#include <RtypesCore.h>

#include <memory>

namespace Compat {

template <typename T>
struct DeprecatedAutoPtr
{
   // We use compat_auto_ptr only to assign the wrapped raw pointer to a unique pointer in an I/O customization rule.
   // Therefore, we don't delete on destruction (because ownership gets transferred to the unique pointer).
   // ~DeprecatedAutoPtr() { delete fRawPtr; }

   T *fRawPtr = nullptr;
};

} // namespace Compat

struct Track {
   int fFoo;

   ClassDefNV(Track, 2)
};

struct Event {
   std::unique_ptr<Track> fTrack;
   float fBar = 137.0;

   ClassDefNV(Event, 3)
};

#endif // EVENT_V3_H
