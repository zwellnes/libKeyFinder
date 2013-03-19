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

#include "chromagramtest.h"

TEST (ChromagramTest, ConstructorDefaultsWork) {
  KeyFinder::Chromagram c;
  ASSERT_EQ(0, c.getHops());
  ASSERT_EQ(0, c.getBands());
}

TEST (ChromagramTest, ConstructorArgumentsWork) {
  KeyFinder::Chromagram c(20, 1, 1);
  ASSERT_EQ(20, c.getHops());
  ASSERT_EQ(12, c.getBands());
  for (int h=0; h<20; h++) {
    for (int b=0; b<12; b++) {
      ASSERT_FLOAT_EQ(0.0, c.getMagnitude(h, b));
    }
  }
}

TEST (ChromagramTest, Mutator) {
  KeyFinder::Chromagram c(1, 1, 1);
  c.setMagnitude(0, 0, 1.0);
  ASSERT_FLOAT_EQ(1.0, c.getMagnitude(0, 0));
}

TEST (ChromagramTest, CopyConstructor) {
  KeyFinder::Chromagram c(1, 1, 1);
  c.setMagnitude(0, 0, 1.0);
  ASSERT_EQ(1, c.getHops());
  ASSERT_EQ(12, c.getBands());
  ASSERT_FLOAT_EQ(1.0, c.getMagnitude(0, 0));
  ASSERT_FLOAT_EQ(0.0, c.getMagnitude(0, 1));

  KeyFinder::Chromagram c2(c);
  ASSERT_EQ( 1, c2.getHops());
  ASSERT_EQ(12, c2.getBands());
  ASSERT_FLOAT_EQ(1.0, c2.getMagnitude(0, 0));
  ASSERT_FLOAT_EQ(0.0, c2.getMagnitude(0, 1));
}

TEST (ChromagramTest, Assignment) {
  KeyFinder::Chromagram c(1, 1, 1);
  c.setMagnitude(0, 0, 1.0);
  ASSERT_EQ(1, c.getHops());
  ASSERT_EQ(12, c.getBands());
  ASSERT_FLOAT_EQ(1.0, c.getMagnitude(0, 0));
  ASSERT_FLOAT_EQ(0.0, c.getMagnitude(0, 1));

  KeyFinder::Chromagram c2 = c;
  ASSERT_EQ( 1, c2.getHops());
  ASSERT_EQ(12, c2.getBands());
  ASSERT_FLOAT_EQ(1.0, c2.getMagnitude(0, 0));
  ASSERT_FLOAT_EQ(0.0, c2.getMagnitude(0, 1));
}

TEST (ChromagramTest, Bounds) {
  KeyFinder::Chromagram c(5, 1, 1);
  // hops min max
  ASSERT_THROW(c.getMagnitude(-1,  0), KeyFinder::Exception);
  ASSERT_THROW(c.getMagnitude( 5,  0), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude(-1,  0, 1.0), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 5,  0, 1.0), KeyFinder::Exception);
  // bands min max
  ASSERT_THROW(c.getMagnitude( 0, -1), KeyFinder::Exception);
  ASSERT_THROW(c.getMagnitude( 0, 12), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 0, -1, 1.0), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 0, 12, 1.0), KeyFinder::Exception);
  // value bounds
  ASSERT_THROW(c.setMagnitude( 0,  0, INFINITY), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 0,  0, NAN), KeyFinder::Exception);
}

TEST (ChromagramTest, Append) {
  KeyFinder::Chromagram c1(5, 1, 1);
  KeyFinder::Chromagram c2(2, 1, 2);
  KeyFinder::Chromagram c3(2, 1, 1);

  // fails when appending chromagram with a different number of bands
  ASSERT_THROW(c1.append(c2), KeyFinder::Exception);

  c1.setMagnitude(4, 0, 1.0);
  c3.setMagnitude(1, 0, 200.0);

  c1.append(c3);
  ASSERT_EQ(7, c1.getHops());
  ASSERT_EQ(12, c1.getBands());
  ASSERT_FLOAT_EQ(1.0, c1.getMagnitude(4, 0));
  ASSERT_FLOAT_EQ(200.0, c1.getMagnitude(6, 0));
}

TEST (ChromagramTest, ReduceToOneOctave) {
  KeyFinder::Chromagram c(1, 3, 1);
  c.setMagnitude(0,  0, 10.0);
  c.setMagnitude(0, 12, 15.0);
  c.setMagnitude(0, 24, 5.0);
  c.reduceToOneOctave();
  ASSERT_FLOAT_EQ(10.0, c.getMagnitude(0, 0));
}

TEST (ChromagramTest, ReduceToOneOctaveDoesntAffectOneOctave) {
  KeyFinder::Chromagram c(1, 1, 1);
  ASSERT_EQ(12, c.getBands());
  c.reduceToOneOctave();
  ASSERT_EQ(12, c.getBands());
}

TEST (ChromagramTest, NilTuning) {
  KeyFinder::Chromagram c(1, 1, 1);
  c.setMagnitude(0,  0, 1.0);

  c.tuningBandAdaptive(0.1);
  ASSERT_EQ(12, c.getBands());
  ASSERT_FLOAT_EQ(1.0, c.getMagnitude(0, 0));

  c.tuningHarte();
  ASSERT_EQ(12, c.getBands());
  ASSERT_FLOAT_EQ(1.0, c.getMagnitude(0, 0));
}

TEST (ChromagramTest, AdaptiveTuning) {
  KeyFinder::Chromagram c(1, 1, 3);
  c.setMagnitude(0,  0,  1.0);
  c.setMagnitude(0,  1,  0.0);
  c.setMagnitude(0,  2, 10.0);

  c.setMagnitude(0,  3,  0.0);
  c.setMagnitude(0,  4, 10.0);
  c.setMagnitude(0,  5,  1.0);

  c.setMagnitude(0,  6,  1.0);
  c.setMagnitude(0,  7, 10.0);
  c.setMagnitude(0,  8,  1.0);

  c.setMagnitude(0,  9, 10.0);
  c.setMagnitude(0, 10,  1.0);
  c.setMagnitude(0, 11,  1.0);

  c.tuningBandAdaptive(0.1);
  ASSERT_EQ(12, c.getBands());
  ASSERT_FLOAT_EQ(10.1, c.getMagnitude(0,0));
  ASSERT_FLOAT_EQ(10.1, c.getMagnitude(0,1));
  ASSERT_FLOAT_EQ(10.2, c.getMagnitude(0,2));
  ASSERT_FLOAT_EQ(10.2, c.getMagnitude(0,3));
}

TEST (ChromagramTest, HarteTuning) {
  KeyFinder::Chromagram c(1, 1, 3);
  c.setMagnitude(0,  0,  1.0);
  c.setMagnitude(0,  1,  1.0);
  c.setMagnitude(0,  2, 10.0);

  c.setMagnitude(0,  3,  1.0);
  c.setMagnitude(0,  4,  1.0);
  c.setMagnitude(0,  5, 10.0);

  c.setMagnitude(0,  6,  1.0);
  c.setMagnitude(0,  7,  1.0);
  c.setMagnitude(0,  8,  0.0);

  c.tuningHarte();
  ASSERT_EQ(12, c.getBands());
  ASSERT_FLOAT_EQ(10.0, c.getMagnitude(0,0));
  ASSERT_FLOAT_EQ(10.0, c.getMagnitude(0,1));
  ASSERT_FLOAT_EQ( 0.0, c.getMagnitude(0,2));
}
