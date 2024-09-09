#ifndef RUN_HH
#define RUN_HH

#include "G4Run.hh"
#include <vector>
#include <map>
#include <tuple>

class Run : public G4Run
{
public:
    Run();
    virtual ~Run();

    virtual void RecordEvent(const G4Event* evt) override;
    virtual void Merge(const G4Run* aRun) override;

    std::vector<std::vector<int>>& GetEventDataH1() { return eventDataH1; }
    const std::vector<std::vector<int>>& GetEventDataH1() const { return eventDataH1; }

    void StoreKineticEnergy(int particleIndex, G4double kineticEnergy);

    std::vector<std::vector<int>> tempKineticEnergyData = std::vector<std::vector<int>>(5, std::vector<int>(100, 0));

private:
    std::vector<std::vector<int>> eventDataH1 = std::vector<std::vector<int>>(5, std::vector<int>(100, 0));
};

#endif // RUN_HH
