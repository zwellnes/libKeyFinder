/*************************************************************************

  Copyright 2011-2013 Ibrahim Sha'ath

  This file is part of LibKeyFinder.

  LibKeyFinder is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LibKeyFinder is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************/

#ifndef TEMPORALWINDOWFACTORY_H
#define TEMPORALWINDOWFACTORY_H

#include <boost/thread/mutex.hpp>
#include <vector>
#include "windowfunctions.h"

namespace KeyFinder {

  class TemporalWindowFactory {
  public:
    TemporalWindowFactory();
    ~TemporalWindowFactory();
    const std::vector<float>* getTemporalWindow(
      unsigned int frameSize,
      temporal_window_t function
    );
  private:
    class TemporalWindowWrapper;
    std::vector<TemporalWindowWrapper*> temporalWindows;
    boost::mutex temporalWindowFactoryMutex;
  };

  class TemporalWindowFactory::TemporalWindowWrapper {
  public:
    TemporalWindowWrapper(
      unsigned int frameSize,
      temporal_window_t function
    );
    unsigned int getFrameSize() const;
    temporal_window_t getFunction() const;
    const std::vector<float>* getTemporalWindow() const;
  private:
    std::vector<float> temporalWindow;
    temporal_window_t function;
  };




}

#endif
