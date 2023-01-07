#pragma once

#include <DwarfUserInterface/Font.h>

#include <DwarvenCore/ObjectPool.h>

namespace df {
    class Renderer;
    class InterfaceRenderData;
    class InterfaceRenderer;
}

namespace df {
    class UserInterface {
    public:
        UserInterface(Renderer& renderer);
        ~UserInterface();

        auto CreateFont(uint32 size, const df::Vector<uint8>& data)->Font*;
        void DestroyFont(Font* font);

        auto GetInterfaceRenderer()->InterfaceRenderer&;

    private:
        Renderer& m_Renderer;
        InterfaceRenderData* m_InterfaceRenderData;
        InterfaceRenderer* m_InterfaceRenderer;

        ObjectPool<Font> m_Fonts;
    };
}