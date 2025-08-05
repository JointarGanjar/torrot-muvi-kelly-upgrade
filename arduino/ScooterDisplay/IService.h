
#ifndef ISERVICE_H
#define ISERVICE_H
// --- DisplayData definition ---
struct DisplayData {
    bool shouldUpdate;
    float value;
};

class IService {
public:
    explicit IService(unsigned long updateIntervalMs) : updateIntervalMs_(updateIntervalMs) {}
    virtual ~IService() = default;
    
    virtual void begin() = 0;
    virtual void serviceLoop(unsigned long now) = 0;
    
    DisplayData* getDisplayData() { return &displayData_; }    
    unsigned long getUpdateInterval() const { return updateIntervalMs_; }

protected:
    DisplayData displayData_ { false, 0.0f };
    const unsigned long updateIntervalMs_;
};


#endif // ISERVICE_H
