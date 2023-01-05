/// @serializable
enum EColored {
    Red, ///< @stringvalue red rote
    Green, ///< @stringvalue green
    Blue, ///< @stringvalue blue
    Yellow,
};

enum class EColor1 {
    Red, ///< @stringvalue red
    Green, ///< @stringvalue green
    Blue, ///< @stringvalue blue
};

/// @serializable
enum class EColor2 {
    Red, ///< @stringvalue red
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
