#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/vfs/buffer.h"

class MockBuffer : public Buffer
{
public:
   MOCK_METHOD(int, read, (void* ptr, int size), (override));
   MOCK_METHOD(int, write, (const void* ptr, int size), (override));
   MOCK_METHOD(char, getchar, (), (override));
   MOCK_METHOD(char, peekchar, (), (override));
   MOCK_METHOD(void, seek, (int pos, int mode), (override));
   MOCK_METHOD(int, tell,(), (const, override));
   MOCK_METHOD(bool, eof, (), (const, override));
   MOCK_METHOD(int, size, (), (override));
};
