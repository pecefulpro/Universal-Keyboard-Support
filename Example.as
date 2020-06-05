public class dlltimer extends Sprite
    {
        public function dlltimer()
        {
            m_ext = new WindowsExtensionWrapper();
            m_ext.addEventListener(ExtensionEvent.PRESS, ext_pressHandler);
            m_ext.addEventListener(ExtensionEvent.RELEASE, ext_releaseHandler);
            m_ext.addEventListener(ExtensionEvent.KEY, ext_keyHandler);
            m_ext.starttimer();
            
            var m_ext : WindowsExtensionWrapper = null;        
            
        }
        

        private function ext_pressHandler(event:ExtensionEvent):void
        {
            var KeyboardID:int = event.result; 
            var KeyPressed:int = event.key;
            trace("Keyboard #" +KeyboardID+ " is pressing key #"+KeyPressed);
        }
        
        private function ext_releaseHandler(event:ExtensionEvent):void
        {
            var KeyboardID:int = event.result; 
            var KeyPressed:int = event.key;
            trace("Keyboard #" +KeyboardID+ " is releasing key #" + KeyPressed);
        }
        
        private function ext_keyHandler(event:ExtensionEvent):void
        {
            var KeyboardID:int = event.result; 
            var KeyPressed:int = event.key;
//            trace("Keyboard #" +KeyboardID+ "Is pressed the key" + KeyPressed );
        }    
    }
