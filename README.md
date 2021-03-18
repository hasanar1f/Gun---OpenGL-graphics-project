# Gun---OpenGL-graphics-project



# Fully Controllable Camera (Problem 1.exe)
Up arrow - move forward
Down arrow - move backward
Right arrow - move right
Left arrow - move left
PgUp - move up
PgDn - move down
1 - rotate/look left
2 - rotate/look right
3 - look up
4 - look down
5 - tilt clockwise
6 - tilt counterclockwise

# Implementation

Maintain four global variables: one 3d point 𝑝𝑜𝑠 to indicate the position of the camera
and three 3d unit vectors 𝑢, 𝑟, 𝑎𝑛𝑑 𝑙 to indicate the up, right, and look directions
respectively. 𝑢, 𝑟, 𝑎𝑛𝑑 𝑙 must be perpendicular to each other, i.e.,
𝑢. 𝑟 = 𝑟. 𝑙 = 𝑙. 𝑢 = 0, 𝑢 = 𝑟 × 𝑙, 𝑙 = 𝑢 × 𝑟, 𝑟 = 𝑙 × 𝑢.You should initialize and maintain the values of 𝑢, 𝑟, 𝑎𝑛𝑑 𝑙 such that the above.

# Gun control

Press the keys q, w, e, r, a, s, d and f to find out how the gun rotates. Also observe that
after certain amount, each joint ceases to rotate.


Left click the mouse to fire the gun.


