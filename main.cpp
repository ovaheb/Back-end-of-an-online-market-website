#include "libraries.hpp"
#include "manager.cpp"

int main()
{
    string commandInput, result;
    Manager JomeBazar;
    while (getline(cin, commandInput))
    {
        try
        {
            result = JomeBazar.processCommand(commandInput);
            cout << result;
        }
        catch(notFound ex)
        {
            cout << "Not Found\n";
        }
        catch(permissionDenied ex)
        {     
            cout << "Permission Denied\n";
        }
        catch(badRequest ex)
        {
            cout << "Bad Request\n";
        }

    }
    return 0;
}