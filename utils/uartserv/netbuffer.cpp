/*
  PLIB - A Suite of Portable Game Libraries
  Copyright (C) 1998,2002  Steve Baker
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
 
  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 
  For further information visit http://plib.sourceforge.net

  $Id: netBuffer.cxx 1568 2002-09-02 06:05:49Z sjbaker $
*/

#include "netbuffer.h"

netBuffer netBufferChannel::tmp_buffer;
netBuffer netBufferChannel::in_buffer;
netBuffer netBufferChannel::out_buffer;
int netBufferChannel::connection_count;
bool netBufferChannel::lossy;

// Notice: the following notice may be incorrect: 
// Notice, this module only reads as much from the network layer as it
// can.  This is loss-less, but it means that the network layer
// buffers could build up, and or the sending side could block.
// However it is up to the next layer up to pull the data out of the
// buffers in a timely manner and decide what to do with it.

void
netBufferChannel::handleRead (void)
{
    if ( !lossy ) {
	// only read in what we have space for in our in_buffer
	int max_read = in_buffer.getMaxLength() - in_buffer.getLength() ;
	if ( max_read ) {
	    char* data = in_buffer.getData() + in_buffer.getLength() ;
	    int num_read = recv (data, max_read) ;
	    if (num_read > 0) {
		in_buffer.append (num_read) ;
		//ulSetError ( UL_DEBUG, "netBufferChannel: %d read", num_read ) ;
	    }
	}
	if (in_buffer.getLength()) {
	    handleBufferRead (in_buffer);
	}
    } else {
	// we need to flush the incoming network packet so things
	// don't build up.  Use a hysterisys based approach.  Once the
	// buffer fills up, it needs to drop to half empty before we
	// will accept more data.  In the mean time we just trash and
	// discard when we can't put in in our in_buffer
	int max_read = in_buffer.getMaxLength() - in_buffer.getLength() ;
	if ( full ) {
	    if ( max_read >= in_buffer.getLength() ) {
		// half full (or less) so accept data again
		full = false;
	    }
	}
	int num_read = recv (tmp_buffer.getData(), tmp_buffer.getMaxLength()) ;
	tmp_buffer.append (num_read);
	//printf("num_read = %d  full = %d\n", num_read, full);
	if ( ! full ) {
	    int bytes = num_read;
	    if ( num_read >= max_read ) {
		bytes = max_read;
		full = true;
	    }
	    in_buffer.append( tmp_buffer.getData(), bytes );
	} else {
	    // just drop the data <snif>
	}
	tmp_buffer.remove();
    }
}

void
netBufferChannel::handleWrite (void)
{
    if (out_buffer.getLength()) {
	if (isConnected()) {
	    int length = out_buffer.getLength() ;
	    if (length>512)
		length=512;
	    int num_sent = netChannel::send (out_buffer.getData(), length);
	    if (num_sent > 0) {
		out_buffer.remove (0, num_sent);
		//ulSetError ( UL_DEBUG, "netBufferChannel: %d sent", num_sent ) ;
	    }
	}
    } else if (should_close) {
	close();
    }
}


/**
 * 
 */
void
netBufferChannel::handleAccept()
{
    netAddress addr;
    int handle = netChannel::accept( &addr );
    printf("Telnet server accepted connection from %s:%d\n",
           addr.getHost(), addr.getPort() );
    netBufferChannel* channel = new netBufferChannel();
    channel->setHandle( handle );
    if ( connection_count > 1 ) {
	const char *msg = "\nThere is already an active connection, closing!\n\n";
	int len = strlen(msg);
	channel->bufferSend(msg, len);
	channel->poll();
	channel->close();
    }
}
