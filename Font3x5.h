class Font3x5 : public Print {
    
  public:

    virtual size_t write(uint8_t); // used by the Arduino Print class
    void setCursor(const int8_t x, const int8_t y);

  private:

    int8_t _cursorX;    // Default is 0.
    int8_t _cursorY;    // Default is 0.
};
