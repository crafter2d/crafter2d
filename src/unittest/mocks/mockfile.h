#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/vfs/file.h"
#include "core/vfs/memorybuffer.h"

#include "mockbuffer.h"

class MockFile : public File
{
public:

   MockFile() {
      ON_CALL(*this, virOpen).WillByDefault([this](const String&, int) {
         this->setBuffer(new MockBuffer());
         return true;
      });
   }

   MockBuffer& getMockBuffer() { return static_cast<MockBuffer&>(getBuffer()); }
   
   MOCK_METHOD(bool, isValid, (), (const, override));
   MOCK_METHOD(bool, virOpen, (const String&, int), (override));
   MOCK_METHOD(void, virClose, (), (override));

private:

   std::unique_ptr<MockBuffer> mBuffer;
};
