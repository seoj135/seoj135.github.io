#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

int lcs() {
    std::string op1 = "ASDBVADASD";
    std::string op2 = "BSFEDDFWWEFASD";    
    
    int* dp = new int[(op1.size() + 1) * (op2.size() + 1)];
    
    int stride = op2.size() + 1;
    std::cout << "op1: " << op1.size() << ", op2: " << op2.size() << std::endl;
    
    for (int r=0; r < op1.size() + 1; r++) 
    {
        for (int c=0; c < stride; c++)
        {
            if (r == 0 || c == 0)
                dp[r * stride + c] = 0;
            else if (op1[r-1] == op2[c-1])
                dp[r * stride + c] = dp[(r-1) * stride + c - 1] + 1;
            else 
                dp[r * stride + c] = std::max(dp[(r-1) * stride + c], dp[r * stride + c - 1]);
            
        }
    }
    
    std::cout << "    ";
    for (int c = 1; c < stride; c++)
        std::cout << op2[c - 1] << " ";
    std::cout << std::endl;
    
    for (int r = 0; r < op1.size() + 1; r++) 
    {
        if (r == 0)
            std:: cout << "  ";
        else 
            std:: cout << op1[r - 1] << " ";
            
        for (int c = 0; c < stride; c++)
            std::cout << dp[r * stride + c] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    // finding str
    std::stringstream ss;
    int r = op1.size();
    int c = stride - 1; 
    while (r > 0 && c > 0) 
    {
        int pos = r * stride + c;
        std::cout << dp[pos] << " ";
        std::cout << dp[pos - 1] << " ";
        std::cout << dp[pos - stride] << "-";
        
        if(op1[r-1] == op2[c-1])
        {
            ss << op1[r-1];
            r--;c--;
        }
        else if(dp[pos - stride] > dp[pos - 1])
            r--;
        else
            c--;
    }
    
    std::cout << std::endl;
    
    std::string lcsStr = ss.str();
    std::reverse(lcsStr.begin(), lcsStr.end());
    std::cout << lcsStr << std::endl;
    
    return dp[(op1.size() + 1) * stride - 1];
}

int main()
{
    std::cout << lcs();
    return 0;
}