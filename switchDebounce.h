
class switchDebounce{
    private:
          
          boolean debounce(boolean reading);
          unsigned long lastDebounceTime = 0;
          boolean travaDebounce = LOW;
          
          int buttonState = HIGH;             
          int lastButtonState = HIGH;    
          
    public:
          boolean state;
          switchDebounce(int pin);
          void refresh(int pin);
          
};


