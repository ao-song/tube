#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "EventHandler.h"
#include "InetAddress.h"
#include "PayloadBuffer.h"

// ----------------------------------------------------------------------------

namespace Tube
{
    class EventHandlerTable;
    class TcpConnectionOwner;

    class TcpConnection : public EventHandler
    {
    public:

        typedef enum
        {
            CallAgain,
            RemoveConnection,
            WaitForEvent
        } Action; 

        typedef enum
        {
            Idle,
            Connecting,
            Established,
            Listen
        } State; 

        TcpConnection(
            EventHandlerTable*  eventHandlerTable,
            TcpConnectionOwner* tcpConnectionOwner);
         
      
        virtual
        ~TcpConnection();
      
      
        virtual
        bool
        accept(
            unsigned short port,
            unsigned int   sendingBufferSize,
            unsigned int   receivingBufferSize);

      
        virtual
        bool
        connect(
            const char*        destinationIp,
            unsigned short     destinationPort,
            unsigned int       sendingBufferSize = 0,
            unsigned int       receivingBufferSize = 0);
      

        virtual
        void
        fin();


        virtual
        TcpConnectionOwner*
        get_connection_owner();      
   
         
      
        virtual
        bool
        handle_event(
            unsigned int event,
            int fd);      

      
        bool
        is_established() const;
         
    
        bool
        is_ok_to_send() const;

        virtual
        Action
        send(
            const void* buffer, 
            size_t      bufferLength);
      
      
        virtual
        Action
        receive(
            void*  buffer, 
            size_t bufferLength);
         

        void
        reset();        
         

        virtual
        Action
        send_buffered_data();

         
    private:
   
      /**
       * Copy constructor not implemented.
       */   
      TcpConnection(
         const TcpConnection& other);

      /**
       * Assignment operator not implemented.
       */
      TcpConnection&
      operator=(
         const TcpConnection& other);

      bool
      make_non_blocking(
          int socket);

      bool
      set_sending_buffer_size(
          unsigned int sendingBufferSize);

      bool
      set_receiving_buffer_size(
          unsigned int receivingBufferSize);
      
   
      TcpConnectionOwner*          connectionOwnerM;
      PayloadBuffer                sendBufferM;
      State                        stateM;
    };

// ----------------------------------------------------------------------------

    inline
    void
    TcpConnection::fin()
    {
        shutdown(get_socket(), SHUT_WR);
    }

    inline
    TcpConnectionOwner*
    TcpConnection::get_connection_owner()
    {
        return connectionOwnerM;
    }

    inline
    bool
    TcpConnection::is_established() const
    {
        return stateM == Established;
    }

    inline
    bool
    TcpConnection::is_ok_to_send() const
    {
        return (stateM == Established) && sendBufferM.is_empty();
    }

}

#endif