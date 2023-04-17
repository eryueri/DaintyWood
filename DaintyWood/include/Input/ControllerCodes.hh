namespace DWE {
namespace Input {
    enum ControllerCodes {

        /*! @defgroup joysticks Joysticks
         *  @brief Joystick IDs.
         *
         *  See [joystick input](@ref joystick) for how these are used.
         *
         *  @ingroup input
         *  @{ */
        JOYSTICK_1 = 0,
        JOYSTICK_2 = 1,
        JOYSTICK_3 = 2,
        JOYSTICK_4 = 3,
        JOYSTICK_5 = 4,
        JOYSTICK_6 = 5,
        JOYSTICK_7 = 6,
        JOYSTICK_8 = 7,
        JOYSTICK_9 = 8,
        JOYSTICK_10 = 9,
        JOYSTICK_11 = 10,
        JOYSTICK_12 = 11,
        JOYSTICK_13 = 12,
        JOYSTICK_14 = 13,
        JOYSTICK_15 = 14,
        JOYSTICK_16 = 15,
        JOYSTICK_LAST = JOYSTICK_16,
        /*! @} */

        /*! @defgroup gamepad_buttons Gamepad buttons
         *  @brief Gamepad buttons.
         *
         *  See @ref gamepad for how these are used.
         *
         *  @ingroup input
         *  @{ */
        BUTTON_A = 0,
        BUTTON_B = 1,
        BUTTON_X = 2,
        BUTTON_Y = 3,
        BUTTON_LEFT_BUMPER = 4,
        BUTTON_RIGHT_BUMPER = 5,
        BUTTON_BACK = 6,
        BUTTON_START = 7,
        BUTTON_GUIDE = 8,
        BUTTON_LEFT_THUMB = 9,
        BUTTON_RIGHT_THUMB = 10,
        BUTTON_DPAD_UP = 11,
        BUTTON_DPAD_RIGHT = 12,
        BUTTON_DPAD_DOWN = 13,
        BUTTON_DPAD_LEFT = 14,
        BUTTON_LAST = BUTTON_DPAD_LEFT,

        BUTTON_CROSS = BUTTON_A,
        BUTTON_CIRCLE = BUTTON_B,
        BUTTON_SQUARE = BUTTON_X,
        BUTTON_TRIANGLE = BUTTON_Y,
        /*! @} */

        /*! @defgroup gamepad_axes Gamepad axes
         *  @brief Gamepad axes.
         *
         *  See @ref gamepad for how these are used.
         *
         *  @ingroup input
         *  @{ */
        AXIS_LEFT_X = 0,
        AXIS_LEFT_Y = 1,
        AXIS_RIGHT_X = 2,
        AXIS_RIGHT_Y = 3,
        AXIS_LEFT_TRIGGER = 4,
        AXIS_RIGHT_TRIGGER = 5,
        AXIS_LAST = AXIS_RIGHT_TRIGGER,
    };
} // namespace Input

} // namespace DWE
