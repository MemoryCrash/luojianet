/*
 * Copyright 2021, 2022 LuoJiaNET Research and Development Group, Wuhan University
 * Copyright 2021, 2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.luojianet_ms.flclient.model;

import com.luojianet_ms.flclient.Common;
import com.luojianet_ms.lite.LiteSession;
import com.luojianet_ms.lite.MSTensor;

import java.util.Map;
import java.util.Optional;
import java.util.logging.Logger;

/**
 * Defining the Callback base class.
 *
 * @since v1.0
 */
public abstract class Callback {
    private static final Logger logger = Logger.getLogger(LossCallback.class.toString());

    LiteSession session;

    public int steps = 0;

    int epochs = 0;

    /**
     * Defining a constructor of  Callback.
     */
    public Callback(LiteSession session) {
        this.session = session;
    }

    protected Optional<MSTensor> searchOutputsForSize(int size) {
        if (session == null) {
            logger.severe(Common.addTag("trainSession cannot be null"));
            return Optional.empty();
        }
        Map<String, MSTensor> outputs = session.getOutputMapByTensor();
        for (MSTensor tensor : outputs.values()) {
            if (tensor == null) {
                logger.severe(Common.addTag("tensor cannot be null"));
                return Optional.empty();
            }
            if (tensor.elementsNum() == size) {
                return Optional.of(tensor);
            }
        }
        logger.severe(Common.addTag("can not find output the tensor,element num is " + size));
        return Optional.empty();
    }

    /**
     * Step begin execute function.
     *
     * @return execute status.
     */
    public abstract Status stepBegin();

    /**
     * Step end execute function.
     *
     * @return execute status.
     */
    public abstract Status stepEnd();

    /**
     * epoch begin execute function.
     *
     * @return execute status.
     */
    public abstract Status epochBegin();

    /**
     * epoch end execute function.
     *
     * @return execute status.
     */
    public abstract Status epochEnd();
}