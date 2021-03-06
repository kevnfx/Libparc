/*
 * Copyright (c) 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** *
 * @author Scott, Glenn <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_AtomicInteger.c"

#include <inttypes.h>
#include <stdio.h>

#include <LongBow/testing.h>
#include <LongBow/debugging.h>

LONGBOW_TEST_RUNNER(test_parc_AtomicInteger)
{
    LONGBOW_RUN_TEST_FIXTURE(Global);
    LONGBOW_RUN_TEST_FIXTURE(Threaded);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(test_parc_AtomicInteger)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(test_parc_AtomicInteger)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicInteger_Uint32Increment);
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicInteger_Uint32Decrement);
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicInteger_Uint64Increment);
    LONGBOW_RUN_TEST_CASE(Global, parcAtomicInteger_Uint64Decrement);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcAtomicInteger_Uint32Increment)
{
    uint32_t value = 0;
    parcAtomicInteger_Uint32Increment(&value);
    assertTrue(value == 1, "Expected 1, actual, %u", value);
}

LONGBOW_TEST_CASE(Global, parcAtomicInteger_Uint32Decrement)
{
    uint32_t value = 0;
    parcAtomicInteger_Uint32Increment(&value);
    assertTrue(value == 1, "Expected 1, actual, %u", value);
}

LONGBOW_TEST_CASE(Global, parcAtomicInteger_Uint64Increment)
{
    uint64_t value = 0;
    parcAtomicInteger_Uint64Increment(&value);
    assertTrue(value == 1, "Expected 1, actual, %"PRIu64"", value);
}

LONGBOW_TEST_CASE(Global, parcAtomicInteger_Uint64Decrement)
{
    uint64_t value = 0;
    parcAtomicInteger_Uint64Increment(&value);
    assertTrue(value == 1, "Expected 1, actual, %"PRIu64"", value);
}

LONGBOW_TEST_FIXTURE(Threaded)
{
    LONGBOW_RUN_TEST_CASE(Threaded, collaborative);
}

LONGBOW_TEST_FIXTURE_SETUP(Threaded)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Threaded)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

static void *
collaborator_A(void *data)
{
    uint32_t *valuePointer = (uint32_t *) data;
    uint32_t contribution = 0;
    while (*valuePointer < 1000000) {
        parcAtomicInteger_Uint32Increment(valuePointer);
        contribution++;
    }
    printf("A contribution %d\n", contribution);
    pthread_exit((void *) NULL);
}

static void *
collaborator_B(void *data)
{
    uint32_t *valuePointer = (uint32_t *) data;

    uint32_t contribution = 0;
    while (*valuePointer < 1000000) {
        parcAtomicInteger_Uint32Increment(valuePointer);
        contribution++;
    }

    printf("B contribution %d\n", contribution);
    pthread_exit((void *) NULL);
}

LONGBOW_TEST_CASE(Threaded, collaborative)
{
    uint32_t value = 0;

    pthread_t thread_A;
    pthread_t thread_B;

    pthread_create(&thread_A, NULL, collaborator_A, &value);
    pthread_create(&thread_B, NULL, collaborator_B, &value);

    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(test_parc_AtomicInteger);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
