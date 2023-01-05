/// @serializable
enum class Color {
    Red,
    Green,
    Blue,
    Cyan,
    Magenta,
    Yellow,
    Black
};

struct OsManager {
    /// \serializable
    enum Type {
        LINUX = 0,
        WINDOWS = 1,
        OSX = 2,
    };
};

namespace The::Longest::Namespace {
class Very {
public:
    class Long {
    public:
        class Qualified {
        public:
            class Name {
            public:
                /**
                 * \brief Just an enum
                 * \date 1999-01-01
                 * \author John Jones
                 * \serializable
                 */
                enum class Value {
                    Foo = 100500,
                };
            };
        };
    };
};
} // namespace The::Longest::Namespace
