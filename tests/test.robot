*** Settings ***
Library    Process
Library    String

*** Test Cases ***
EX01 Basic Calculation 1
    Assert Calculation Result 8 9 * 9 - 9 - 9 - 4 - 1 +

EX02 Basic Sort Test
    Assert Sorted Result    3    1    2    5    4

EX02 Already Sorted Test
    Assert Sorted Result    1    2    3    4    5

EX02 Negative values Test
    Assert Error Result    -1    2   0    9    100

EX02 Character input Test
    Assert Error Result    100    a    3    4    6

EX02 INT_MAX input Test
    Assert Error Result    134314134134134    9    3    4    6

*** Keywords ***
Assert Calculation Result
    [Arguments]    @{input}

    ${result}=    Run Process    ../ex01/RPN

    Should Be Equal As Integers    ${result.rc}    0

    ${actual}=    42

    Should Be Equal As Integers    ${actual}    ${result}
    
Assert Sorted Result
    [Arguments]    @{input}

    ${result}=    Run Process    ../ex02/PmergeMe    @{input}

    Should Be Equal As Integers    ${result.rc}    0

    ${after_lines}=    Evaluate
    ...    [l for l in """${result.stdout}""".splitlines() if l.startswith("After:")]

    Should Not Be Empty    ${after_lines}

    ${numbers}=    Replace String    ${after_lines[0]}    After:    ${EMPTY}
    ${numbers}=    Strip String    ${numbers}

    @{actual}=    Split String    ${numbers}

    ${expected}=    Evaluate    sorted(map(int, $input))

    ${actual}=      Evaluate    list(map(int, $actual))

    Should Be Equal    ${actual}    ${expected}

Assert Error Result
    [Arguments]    @{input}

    ${result}=    Run Process    ../ex02/PmergeMe    @{input}

    Should Be Equal As Integers    ${result.rc}    1
    Should Contain    ${result.stderr}    Error