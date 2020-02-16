#pragma once

#include <memory>
#include <GeoPlanetLib/Region.h>

namespace gp
{
    class Surface;

    typedef std::vector<std::shared_ptr<TectonicPlate>> PlateList;

    class TectonicPlate
    {
    public:

        // static methods
        static TectonicPlate *getPlateOfRegion(std::shared_ptr<Region> region);

        /**
         * @brief Assigns a tectonic plate to region as RegionAttribute.
         *
         * @param plate         pointer to tectonic plate to be assigned to region
         * @param region        region which will be given a tectonic plate
         * @param forceOverride flag forcing a override to potential existing plate on region.
         * @return true         if region didn't have a tetonic plate assigned or forceOverride flag was set to true.
         * @return false        if region already had a tectonic plate and forceOverride was set to false.
         */
        static bool assignPlateToRegion(TectonicPlate *plate, std::shared_ptr<Region> region, bool forceOverride = false);

        static void removePlateFromRegion(std::shared_ptr<Region> region);

        static void removePlatesFromSurface(std::shared_ptr<Surface> surface);

        // Methods
        TectonicPlate(std::shared_ptr<Surface> surface) : surface(surface) {}

        inline const RegionList& getMemberRegions() const { return memberRegions; }
        inline const RegionList& getEdgeRegions() const { return edgeRegions; }

        /**
         * @brief Assigns given region to this plate if it's not member of another plate
         *
         * @param region
         * @return true if region was successfully assigned
         * @return false if region was refused due to having a region already
         */
        bool addRegion(std::shared_ptr<Region> region);

        bool expand();

    private:
        // Properties
        RegionList memberRegions;
        RegionList edgeRegions;
        std::shared_ptr<Surface> surface;
        bool expansionFinished = false;
    };
}