
#ifndef CONTENT_READER_H
#define CONTENT_READER_H

namespace Graphics
{
   class Device;
}

class IContent;
class ContentManager;
class DataStream;
class Simulator;

class IContentReader
{
public:
   IContentReader();

 // get/set
   bool              hasGraphicsDevice() const;
   Graphics::Device& getGraphicsDevice();
   void              setGraphicsDevice(Graphics::Device& device);

   bool       hasPhysicsSimulator() const;
   Simulator& getPhysicsSimulator();
   void       setPhysicsSimulator(Simulator& simulator);

   /// Reads and instantiates the content from disc
   virtual IContent* read(ContentManager& manager, DataStream& stream) = 0;

private:

 // data
   Graphics::Device* mpDevice;
   Simulator*        mpSimulator;
};

#endif // CONTENT_READER_H
