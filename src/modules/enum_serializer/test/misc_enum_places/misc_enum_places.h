/// @serializable
enum class Color {
    Red, ///< @stringvalue red rote
    Green, ///< @stringvalue green
    Blue, ///< @stringvalue blue
};

struct OsManager {
    /// \serializable
    enum Type {
        LINUX = 0,
        WINDOWS = 1,
        OSX = 2,
    };
};
