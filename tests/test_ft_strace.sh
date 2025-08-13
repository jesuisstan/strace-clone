#!/bin/bash

# Test script for ft_strace
# Tests basic Linux commands to verify ft_strace functionality

set -e

echo "🧪 Testing ft_strace with basic Linux commands..."
echo "=================================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to run test
run_test() {
    local test_name="$1"
    local command="$2"
    
    echo -e "\n${YELLOW}Testing: $test_name${NC}"
    echo "Command: $command"
    echo "--- ft_strace output ---"
    
    # Run ft_strace and capture output
    if .././ft_strace $command 2>&1; then
        echo -e "${GREEN}✅ $test_name: PASSED${NC}"
    else
        echo -e "${RED}❌ $test_name: FAILED${NC}"
        return 1
    fi
}

# Test 1: Basic command (echo)
run_test "Basic echo command" "echo 'Hello, ft_strace!'"

# Test 2: File operations (ls)
run_test "File listing (ls)" "ls -la"

# Test 3: System information (uname)
run_test "System info (uname)" "uname -a"

# Test 4: File content (cat)
run_test "File reading (cat)" "cat /etc/passwd"

# Test 5: Directory operations (pwd, mkdir, rmdir)
run_test "Directory operations" "pwd"

# Test 6: Process information (ps simple)
run_test "Process listing (ps)" "ps"

# Test 7: User information (whoami)
run_test "User info (whoami)" "whoami"

echo -e "\n${GREEN}🎉 All tests completed!${NC}"
echo "=================================================="
