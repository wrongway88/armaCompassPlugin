class CfgPatches 
{
    class CompassClient
    {
        units[] = {};
        weapons[] = {};
        author = "Manuel";
        
        requiredVersion = 1.60;
        requiredAddons[] = {"Extended_EventHandlers", "cba_main"};
    };
};

class Extended_PreInit_EventHandlers 
{
    CompassClient_INIT = "[] execVM '\CompassClientMod\compassClient.sqf'";
};