#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#define SCROLL_SPEED 10.0f

#if DEBUG
#define Assert( expression ) if( !( expression ) ) {*(int * )0 = 0; }
#else
#define Assert( expression )
#endif


#endif //GLOBALS_HPP
