#pragma once

namespace Plteen {
    class IPlane;
    class IMatter;
    class ISprite;
    class Tracklet;
    class Cosmos;
    class Planelet;
    class RGBA;

    enum class SpeechBubble { Default, Thought };
    enum class BorderEdge { TOP, RIGHT, BOTTOM, LEFT, NONE };
    enum class BorderStrategy { IGNORE, STOP, BOUNCE };
    enum class Log { None, Fatal, Error, Warning, Info, Debug };    
}
