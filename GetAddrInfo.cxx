///:
/*****************************************************************************
 **                                                                         **
 **                               .======.                                  **
 **                               | INRI |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                      .========'      '========.                         **
 **                      |   _      xxxx      _   |                         **
 **                      |  /_;-.__ / _\  _.-;_\  |                         **
 **                      |     `-._`'`_/'`.-'     |                         **
 **                      '========.`\   /`========'                         **
 **                               | |  / |                                  **
 **                               |/-.(  |                                  **
 **                               |\_._\ |                                  **
 **                               | \ \`;|                                  **
 **                               |  > |/|                                  **
 **                               | / // |                                  **
 **                               | |//  |                                  **
 **                               | \(\  |                                  **
 **                               |  ``  |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                   \\    _  _\\| \//  |//_   _ \// _                     **
 **                  ^ `^`^ ^`` `^ ^` ``^^`  `^^` `^ `^                     **
 **                                                                         **
 **                    Copyright © 1997-2013 by Tong G.                     **
 **                          ALL RIGHTS RESERVED.                           **
 **                                                                         **
 ****************************************************************************/

#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>

#include "DieMessage.hh"

void _PrintSocketAddress( struct sockaddr const*, FILE* );

int main( int _Argc, char* *_Argv )
    {
    if ( _Argc != 3 )
        _DieWithUserMessage( "Parameter(s)", "<Address/Name> <Port/Service>" );

    std::string _AddrString = _Argv[1]; // Server address/name
    std::string _PortString = _Argv[2]; // Server port/service

    // Tell the system what kind(s) of address info we want
    struct addrinfo _AddrCriteria;
    std::memset( &_AddrCriteria, 0, sizeof( _AddrCriteria ) );
    _AddrCriteria.ai_family = AF_UNSPEC;        // Any address family
    _AddrCriteria.ai_socktype = SOCK_STREAM;    // Only stream sockets
    _AddrCriteria.ai_protocol = IPPROTO_TCP;    // Only Protocol

    // Get address(es) associated with the specified name/service
    struct addrinfo* _AddrList;

    // Modify _ServAddr contents to reference linked list of addresses
    int _RtnVal = getaddrinfo( _AddrString.c_str()
                             , _PortString.c_str()
                             , &_AddrCriteria
                             , &_AddrList
                             );
    if ( _RtnVal != 0 )
        _DieWithUserMessage( "getaddrinfo() failed"
                           , gai_strerror( _RtnVal )
                           );

    for ( struct addrinfo* _Addr = _AddrList;
          _Addr != nullptr;
          _Addr = _Addr->ai_next )
        {
        _PrintSocketAddress( _Addr->ai_addr, stdout);

        std::cout << std::endl;
        }

    freeaddrinfo( _AddrList );
#
    return 0;
    }

void _PrintSocketAddress( struct sockaddr const* _Address
                        , FILE* _Stream
                        )
    {
    // Test for address and stream
    if ( _Address == nullptr || _Stream == nullptr )
        return;

    void* _NumericAddress = nullptr;    // Pointer to binary address

    // Buffer to contain result ( IPv6 sufficient to hold IPv4 )
    char _AddrBuffer[ INET6_ADDRSTRLEN ];
    in_port_t _Port;    // Port to print

    // Set pointer to address
    switch ( _Address->sa_family )
        {
    case AF_INET:
        _NumericAddress = &( ( struct sockaddr_in* ) _Address )->sin_addr;
        _Port = ntohs( ( ( struct sockaddr_in* ) _Address )->sin_port );
        break;

    case AF_INET6:
        _NumericAddress = &( ( struct sockaddr_in6* ) _Address )->sin6_addr;
        _Port = ntohs( ( ( struct sockaddr_in6* ) _Address )->sin6_port );
        break;

    default:
        std::fputs( "[unknown type]", _Stream );
        return;
        }

    // Convert binary to printable address
    if ( inet_ntop( _Address->sa_family
                  , _NumericAddress
                  , _AddrBuffer
                  , sizeof( _AddrBuffer )
                  ) == nullptr )
        fputs( "[invalid address]", _Stream );
    else
        {
        std::fprintf( _Stream, "%s", _AddrBuffer );
        if ( _Port )
            std::fprintf( _Stream, "-%u", _Port );
        }
    }

 ////////////////////////////////////////////////////////////////////////////

 /***************************************************************************
 **                                                                        **
 **      _________                                      _______            **
 **     |___   ___|                                   / ______ \           **
 **         | |     _______   _______   _______      | /      |_|          **
 **         | |    ||     || ||     || ||     ||     | |    _ __           **
 **         | |    ||     || ||     || ||     ||     | |   |__  \          **
 **         | |    ||     || ||     || ||     ||     | \_ _ __| |  _       **
 **         |_|    ||_____|| ||     || ||_____||      \________/  |_|      **
 **                                           ||                           **
 **                                    ||_____||                           **
 **                                                                        **
 ***************************************************************************/
///:~



