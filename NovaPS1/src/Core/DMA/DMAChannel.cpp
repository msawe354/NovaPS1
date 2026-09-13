#include "DMAChannel.hpp"

namespace nova::ps1 {

DMAChannel::DMAChannel()
    : _baseAddress(0), _blockControl(0), _channelControl(0),
      _enabled(false), _triggered(false) {
}

void DMAChannel::Reset() {
    _baseAddress = 0;
    _blockControl = 0;
    _channelControl = 0;
    _enabled = false;
    _triggered = false;
}

}