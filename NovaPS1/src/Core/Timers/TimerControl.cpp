#include "TimerControl.hpp"

namespace nova::ps1 {

TimerControl::TimerControl() : _mode(0) {
}

void TimerControl::Reset() {
    _mode = 0;
}

}